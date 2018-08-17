#include "MainMenu.h"

MainMenu::MainMenu(Window* window, GraphicsResourceFactory* graphicsResourceFactory, GraphicsContext * graphicsContext, ResourceManager * resourceManager, GUILayout * mainGUILayout)
	: Scene(graphicsContext, resourceManager), 
	m_window(window),
	m_mainGUILayout(mainGUILayout), 
	m_graphicsResourceFactory(graphicsResourceFactory),
	m_mainMenuGUILayout(new GUILayout()),
	m_newGameButton(new GUIButton()),
	m_exitButton(new GUIButton()),
	m_lastCursorState(CursorType::Default)

{
	m_mainMenuGUILayout->setPosition(0, 0);
	m_mainMenuGUILayout->setSize(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight());
	m_mainMenuGUILayout->enableBackgroundRendering();
	
	Texture* backgroundImage = m_resourceManager->load<Texture>("resources/textures/gui/mainmenu_bg.jpg");
	backgroundImage->setMinificationFilter(Texture::Filter::Linear);

	m_mainMenuGUILayout->setBackgroundImage(backgroundImage);

	Texture* newGameButton = m_resourceManager->load<Texture>("resources/textures/gui/newgame_btn.png");
	newGameButton->generateMipMaps();

	Texture* newGameButtonHover = m_resourceManager->load<Texture>("resources/textures/gui/newgame_btn_hover.png");
	newGameButtonHover->generateMipMaps();

	m_newGameButton->setImage(newGameButton);
	m_newGameButton->setHoverImage(newGameButtonHover);
	m_newGameButton->setSize(256, 64);
	m_newGameButton->setPosition(m_graphicsContext->getViewportWidth() / 2 - 128, 245);
	m_mainMenuGUILayout->addWidget(m_newGameButton);

	Texture* exitButton = m_resourceManager->load<Texture>("resources/textures/gui/exit_btn.png");
	newGameButton->generateMipMaps();

	Texture* exitButtonHover = m_resourceManager->load<Texture>("resources/textures/gui/exit_btn_hover.png");
	exitButtonHover->generateMipMaps();

	m_exitButton->setImage(exitButton);
	m_exitButton->setHoverImage(exitButtonHover);
	m_exitButton->setSize(256, 64);
	m_exitButton->setPosition(m_graphicsContext->getViewportWidth() / 2 - 128, 324);
	m_mainMenuGUILayout->addWidget(m_exitButton);

	m_text = new GUIText(m_graphicsResourceFactory);
	m_text->setPosition(m_window->getWidth() - 270, m_window->getHeight() - 35);
	m_text->setFont(m_resourceManager->getResource<Font>("fonts_tuffy"));
	m_text->setFontSize(10);
	m_text->setText("Powered by StarWind Engine team");
	m_text->setColor(1.0, 1.0, 1.0);

	m_mainMenuGUILayout->addWidget(m_text);
}

MainMenu::~MainMenu()
{
	if (m_newGameButton != nullptr) {
		delete m_newGameButton;
		m_newGameButton = nullptr;
	}

	if (m_exitButton != nullptr) {
		delete m_exitButton;
		m_exitButton = nullptr;
	}

	if (m_mainMenuGUILayout != nullptr) {
		delete m_mainMenuGUILayout;
		m_mainMenuGUILayout = nullptr;
	}

	delete m_text;
}

void MainMenu::update()
{

}

void MainMenu::render()
{

}

void MainMenu::activate()
{
	m_lastCursorState = m_window->getCursorType();

	m_window->setCursorType(CursorType::Default);
	m_mainGUILayout->addWidget(m_mainMenuGUILayout);
}
 
void MainMenu::deactivate()
{
	m_mainGUILayout->removeWidget(m_mainMenuGUILayout);
	m_window->setCursorType(m_lastCursorState);
}

void MainMenu::onButtonClick(ButtonType buttonType, const GUIButton::ClickCallback & callback)
{
	if (buttonType == ButtonType::NewGame)
		m_newGameButton->onClick(callback);
	else if (buttonType == ButtonType::Exit)
		m_exitButton->onClick(callback);
}