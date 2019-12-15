#include "GameApplication.h"

#include <Engine/Exceptions/EngineRuntimeException.h>

#include <spdlog/spdlog.h>

#include "Game/Game/PlayerComponent.h"

GameApplication::GameApplication(int argc, char* argv[])
    : BaseGameApplication(argc, argv, "Game", 640, 480)
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
    m_screenManager->changeScreen(BaseGameScreen::getScreenName(GameScreenType::Game));
}
