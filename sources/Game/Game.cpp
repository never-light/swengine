#include "Game.h"
#include "config.h"
#include "Graphics\SolidMeshLoader.h"

#include <Windows.h>

#include <Engine\Utils\string.h>
#include <Engine\Utils\time.h>

Game::Game(const std::string& windowName, unsigned int width, unsigned int height) 
	: BaseGame(windowName, width, height),
	InputEventsListener()
{
	double engineStartLoadingTime = TimeUtils::getCurrentTime();

	m_console = new Console();
	m_console->registerCommandHandler("ftime", [this](Console* console, auto args) {
		double time = this->m_lastFrameTime;

		if (!args.empty() && args[0] == "-max")
			time = this->m_maxFrameTime;

		console->print(std::to_string(this->m_lastFrameTime));
	});

	m_console->registerCommandHandler("viewport-size", [this](Console* console, auto args) {
		console->print(StringUtils::format("%dx%d", this->m_window->getWidth(), this->m_window->getHeight()));
	});

	m_console->registerCommandHandler("file-available", [this](Console* console, auto args) {
		if (args.empty())
			return;

		std::string filename = args.front();

		if (std::experimental::filesystem::exists(filename))
			console->print("true");
		else
			console->print("false");
	});

	m_resMgr->load<Font>("resources/fonts/tuffy.font", "fonts_tuffy");
	m_guiConsoleWidget = new GUIConsoleWidget(m_console, m_resMgr->getResource<Font>("fonts_tuffy"), m_window->getWidth(), m_graphicsSystem->getResourceFactory(), m_graphicsContext);
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

	m_guiConsoleWidget->hide();

	m_console->print("[LOG] Start engine loading...");


	m_inputMgr->registerEventListener(this);

	m_resMgr->registerResourceLoader(new SolidMeshLoader(m_resMgr, m_graphicsSystem->getResourceFactory()), "mod");


	m_startScene = new StartScene(m_graphicsContext, m_resMgr, m_inputMgr);
	SceneId startSceneId = m_sceneMgr->registerScene(m_startScene);

	m_mainMenu = new MainMenu(m_window, m_graphicsSystem->getResourceFactory(), m_graphicsContext, m_resMgr, m_guiMgr->getMainLayout());
	m_mainMenu->onButtonClick(MainMenu::ButtonType::Exit, [this](const MousePosition& mousePosition) {
		this->m_window->close();
	});
	m_mainMenu->onButtonClick(MainMenu::ButtonType::NewGame, [this, startSceneId](const MousePosition& mousePosition) {
		this->m_sceneMgr->setActiveScene(startSceneId);
	});

	SceneId mainMenuSceneId = m_sceneMgr->registerScene(m_mainMenu);

	m_sceneMgr->setActiveScene(mainMenuSceneId);

	m_guiConsoleWidget->setPosition(0, 0);

	m_guiMgr->getMainLayout()->addWidget(m_guiConsoleWidget);

	m_console->print(StringUtils::format("[LOG] Engine is loaded [time: %.2fs]", TimeUtils::getCurrentTime() - engineStartLoadingTime));
}

Game::~Game() {
	delete m_console;
	delete m_guiConsoleWidget;
	delete m_mainMenu;
	delete m_startScene;
}

void Game::update() {
	m_inputMgr->update();
	m_sceneMgr->update();
	m_guiMgr->update();
	m_window->update();
}

void Game::render() {
	m_graphicsContext->clear(vector3(0.6, 0.6, 0.8));

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
}