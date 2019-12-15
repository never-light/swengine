#include "GameApplication.h"

#include <spdlog/spdlog.h>
#include <Engine/Exceptions/EngineRuntimeException.h>

#include "Game/Screens/GameScreen.h"
#include "Game/Screens/MainMenuScreen.h"

GameApplication::GameApplication(int argc, char* argv[])
    : BaseGameApplication(argc, argv, "Game", 1280, 720)
{

}

GameApplication::~GameApplication()
{

}

void GameApplication::render()
{
}

void GameApplication::load()
{
    auto resourceMgr = m_resourceManagementModule->getResourceManager();
    resourceMgr->addResourcesMap("../resources/resources.xml");

    m_screenManager->registerScreen(std::make_shared<GameScreen>(m_inputModule));
    m_screenManager->registerScreen(std::make_shared<MainMenuScreen>(m_inputModule, m_gameConsole));

    m_screenManager->changeScreen(BaseGameScreen::getScreenName(GameScreenType::MainMenu));
}
