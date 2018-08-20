#include "GameHUD.h"

GameHUD::GameHUD(GraphicsContext* graphicsContext, 
	GraphicsResourceFactory* graphicsResourceFactory, 
	Font* defaultFont, 
	GUIManager * guiManager, 
	GUILayout * guiLayout)
	: m_guiManager(guiManager),
	m_guiLayout(guiLayout),
	m_graphicsContext(graphicsContext),
	m_graphicsResourceFactory(graphicsResourceFactory),
	m_defaultFont(defaultFont),
	m_isControlLocked(false),
	m_isActiveTaskExists(false)
{
	m_currentTaskText = new GUIText(m_graphicsResourceFactory);
	m_currentTaskText->setPosition(120, 50);
	m_currentTaskText->setFont(m_defaultFont);
	m_currentTaskText->setFontSize(14);
	m_currentTaskText->setColor(1.0, 1.0, 1.0);
	m_currentTaskText->hide();

	m_guiLayout->addWidget(m_currentTaskText);

	m_currentObjectiveText = new GUIText(m_graphicsResourceFactory);
	m_currentObjectiveText->setPosition(135, 85);
	m_currentObjectiveText->setFont(m_defaultFont);
	m_currentObjectiveText->setFontSize(11);
	m_currentObjectiveText->setColor(1.0, 1.0, 1.0);
	m_currentObjectiveText->hide();

	m_guiLayout->addWidget(m_currentObjectiveText);
	m_interactiveHint = new GUIText(m_graphicsResourceFactory);
	m_interactiveHint->setPosition(m_guiLayout->getWidth() / 2, m_guiLayout->getHeight() - 185);
	m_interactiveHint->setFont(m_defaultFont);
	m_interactiveHint->setFontSize(12);
	m_interactiveHint->setColor(1.0, 1.0, 1.0);
	m_interactiveHint->hide();

	m_guiLayout->addWidget(m_interactiveHint);

	m_codePanel = new CodePanel(m_graphicsContext, m_graphicsResourceFactory, m_defaultFont);
	m_codePanel->setPosition(m_guiLayout->getWidth() / 2 - m_codePanel->getWidth() / 2,
		m_guiLayout->getHeight() / 2 - m_codePanel->getHeight() / 2);

	m_codePanel->hide();

	registerModalWindow(m_codePanel);
}

GameHUD::~GameHUD()
{
	delete m_codePanel;

	delete m_interactiveHint;
	delete m_currentTaskText;
	delete m_currentObjectiveText;
}

void GameHUD::update()
{
}

void GameHUD::setCurrentTaskInfo(const std::string & taskTitle, const std::string & objectiveTitle)
{
	m_currentTaskText->setText(taskTitle);
	m_currentObjectiveText->setText(objectiveTitle);

	m_isActiveTaskExists = true;
}

void GameHUD::setNoActiveTask()
{
	m_currentTaskText->setText("Активных задач нет");
	m_currentObjectiveText->hide();

	m_isActiveTaskExists = false;
}

void GameHUD::showTaskInfo()
{
	m_currentTaskText->show();

	if (m_isActiveTaskExists)
		m_currentObjectiveText->show();
}

void GameHUD::hideTaskInfo()
{
	m_currentTaskText->hide();
	m_currentObjectiveText->hide();
}

void GameHUD::setInteractiveObjectHint(const std::string & hint)
{
	m_interactiveHint->setText(hint);
}

void GameHUD::showInteractiveObjectHint()
{
	m_interactiveHint->show();
}

void GameHUD::hideInteractiveObjectHint()
{
	m_interactiveHint->hide();
}

void GameHUD::registerModalWindow(HUDWindow * window)
{
	window->setChageStateInternalCallbacks(std::bind(&GameHUD::openModalWindowCallback, this, std::placeholders::_1),
		std::bind(&GameHUD::closeModalWindowCallback, this, std::placeholders::_1));

	m_guiLayout->addWidget(window);
}

bool GameHUD::isControlLocked() const
{
	return m_isControlLocked;
}

void GameHUD::lockControl()
{
	m_isControlLocked = true;
}

void GameHUD::unlockControl()
{
	m_isControlLocked = false;
}

unsigned int GameHUD::getScreenWidth() const
{
	return m_guiLayout->getWidth();
}

unsigned int GameHUD::getScreenHeight() const
{
	return m_guiLayout->getHeight();
}

Font * GameHUD::getDefaultFont() const
{
	return m_defaultFont;
}

CodePanel * GameHUD::getCodePanelWindow() const
{
	return m_codePanel;
}

void GameHUD::openModalWindowCallback(HUDWindow * window)
{
	m_activeWindow = window;

	m_guiManager->setCursorType(CursorType::Default);
	lockControl();
}

void GameHUD::closeModalWindowCallback(HUDWindow * window)
{
	m_activeWindow = nullptr;

	m_guiManager->setCursorType(CursorType::Hidden);
	unlockControl();
}
