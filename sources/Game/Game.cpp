#include "Game.h"
#include "config.h"

#include <Windows.h>

#include <Engine/Utils/string.h>
#include <Engine/Utils/time.h>
#include <Engine/Utils/io.h>

#include "Exceptions/FatalErrorException.h"

Game::Game(const std::string& windowName, unsigned int width, unsigned int height) 
	: BaseGame(windowName, width, height),
	InputEventsListener()
{
	double engineStartLoadingTime = TimeUtils::getCurrentTime();

	m_inputMgr->registerEventListener(this);

	initializeInternalConsole();

	// Graphics 
	m_graphicsSystem = new GraphicsSystem(m_window, GraphicsAPI::OpenGL33, m_consoleLogger);
	m_graphicsContext = m_graphicsSystem->getGraphicsContext();

	// Resource Manager
	m_resMgr = new ResourceManager(m_graphicsSystem->getGraphicsContext());

	preLoadCommonResources();

	// GUI Manager
	m_guiMgr = new GUIManager(m_window, m_inputMgr, m_graphicsContext,
		m_resMgr->getResource<GpuProgram>("gpu_programs_gui_program").getRawPtr());

	m_guiMgr->setCurrentCursor(m_defaultGameCursor);

	initializeConsoleGUI();

	m_console->print("[LOG] Start engine loading...");

	loadScenes();
	m_sceneMgr->setActiveScene(m_mainMenu->getId());

	m_guiMgr->getMainLayout()->addWidget(m_guiConsoleWidget);
	m_console->print(StringUtils::format("[LOG] Engine is loaded [time: %.2fs]", TimeUtils::getCurrentTime() - engineStartLoadingTime));
}

Game::~Game() {
	delete m_defaultGameCursor;

	delete m_consoleLogger;
	delete m_consoleCommandsHandler;
	delete m_console;
	delete m_guiConsoleWidget;

	delete m_mainMenu;
	delete m_startScene;

	delete m_guiMgr;

	delete m_resMgr;
	delete m_graphicsSystem;
}

void Game::update() {
	m_soundSystem->update(1.0f/30);
	m_inputMgr->update();

	if (!m_guiConsoleWidget->isVisible())
		m_sceneMgr->update();
	
	m_guiMgr->update();
	m_window->update();
}

void Game::render() {
	m_graphicsContext->setClearColor(0.6f, 0.6f, 0.8f);
	m_graphicsContext->clear(RenderTarget::CLEAR_COLOR | RenderTarget::CLEAR_DEPTH);

	m_sceneMgr->render();
	m_guiMgr->render();

	m_graphicsContext->swapBuffers();
}

void Game::onKeyPress(Key key, KeyEvent::Modifier mod) {
	if (key == GLFW_KEY_ESCAPE) {
		m_sceneMgr->setActiveScene(m_mainMenu->getId());
	}

	if (key == GLFW_KEY_GRAVE_ACCENT) {
		if (m_guiConsoleWidget->isVisible()) {
			m_guiConsoleWidget->hide();
		}
		else {
			m_guiConsoleWidget->show();
		}
	}

	if (mod == KeyEvent::Modifier::Control) {
		auto hotkeyCommandIt = m_consoleCommandsHotkeys.find(key);

		if (hotkeyCommandIt != m_consoleCommandsHotkeys.end())
			m_console->executeCommand(hotkeyCommandIt->second);
	}
}

void Game::preLoadCommonResources()
{
	try {
		m_resMgr->loadResource<Font>("resources/fonts/tuffy.font", "fonts_tuffy").getRawPtr();
		m_resMgr->loadResource<GpuProgram>("resources/shaders/gui/quadwidget.fx", "gpu_programs_gui_program").getRawPtr();

		m_defaultGameCursor = new Cursor(*m_resMgr->loadResource<RawImage>("resources/textures/cursors/default.png", "images_cursors_default").getRawPtr());
	}
	catch (const ResourceLoadingException& exception) {
		processResourceLoadingError(exception);
	}
}

void Game::initializeInternalConsole()
{
	m_console = new Console();
	m_consoleCommandsHandler = new ConsoleCommandsHandler(m_console);
	m_consoleLogger = new ConsoleLogger(m_console);

	addConsoleCommandHotkey(GLFW_KEY_O, "set_camera free");
	addConsoleCommandHotkey(GLFW_KEY_P, "set_camera fps");
}

void Game::initializeConsoleGUI()
{
	m_guiConsoleWidget = new GUIConsoleWidget(m_console, 
		m_resMgr->getResource<Font>("fonts_tuffy").getRawPtr(), m_window->getWidth(), m_graphicsContext);
	m_guiConsoleWidget->setPaddingTop(5);
	m_guiConsoleWidget->setPaddingLeft(10);

	m_guiConsoleWidget->setFontSize(8);
	m_guiConsoleWidget->setBackgroundColor(vector4(0.168f, 0.172f, 0.25f, 0.8f));
	m_guiConsoleWidget->setTextColor(vector4(1.0f, 1.0f, 1.0f, 1.0f));
	m_guiConsoleWidget->setInputTextBoxBackgroundColor(vector4(0.118f, 0.112f, 0.15f, 1.0f));
	m_guiConsoleWidget->setInputTextBoxColor(vector4(1.0f, 1.0f, 1.0f, 1.0f));

	m_guiConsoleWidget->setTextBoxMargin(7);
	m_guiConsoleWidget->setMaxTextLines(20);
	m_guiConsoleWidget->setTextLineHeight(16);

	m_guiConsoleWidget->setZIndex(10);

	m_guiConsoleWidget->setPosition(0, 0);
	m_guiConsoleWidget->hide();

	m_consoleCommandsHandler->attachGUIConsoleWidget(m_guiConsoleWidget);
}

void Game::addConsoleCommandHotkey(Key key, const std::string & command)
{
	m_consoleCommandsHotkeys.insert({ key, command });
}

void Game::loadScenes()
{
	try {
		m_startScene = new LevelScene(m_graphicsContext, m_resMgr, m_inputMgr, m_guiMgr, m_console);

		m_sceneMgr->registerScene(m_startScene);

		m_mainMenu = new MainMenu(m_window, m_graphicsContext, m_resMgr, m_guiMgr->getMainLayout());
		m_sceneMgr->registerScene(m_mainMenu);
	}
	catch (const ResourceLoadingException& exception) {
		processResourceLoadingError(exception);
	}

	m_mainMenu->onButtonClick(MainMenu::ButtonType::Exit, [this](const MousePosition& mousePosition) {
		this->m_window->close();
	});

	m_mainMenu->onButtonClick(MainMenu::ButtonType::NewGame, [this](const MousePosition& mousePosition) {
		this->m_sceneMgr->setActiveScene(this->m_startScene->getId());
	});
}

void Game::processResourceLoadingError(const ResourceLoadingException & error)
{
	std::string errorType = "";

	switch (error.getError()) {
	case ResourceLoadingError::FileNotAvailable:
		errorType = "File is not available";
		break;

	case ResourceLoadingError::InvalidData:
		errorType = "Invalid resource data";
		break;

	case ResourceLoadingError::InvalidType:
		errorType = "Invalid resource type";
		break;

	default:
		errorType = "Unknown internal error";
		break;
	}

	std::string errorMessage = std::string() + "Resource loading error\n" +
		"Resource: " + error.getResourceName() + "\n" +
		"Error type: " + errorType + "\n" +
		"Error description: " + error.what();

	throw FatalErrorException(errorMessage.c_str(), error.getFile(), error.getLine(), error.getFunction());
}