#include "GameApplication.h"

#include <spdlog/spdlog.h>
#include <Engine/Exceptions/EngineRuntimeException.h>
#include <Engine/Modules/Graphics/Resources/SkeletonResource.h>

#include "Game/Screens/GameScreen.h"
#include "Game/Screens/MainMenuScreen.h"

#include <glm/gtx/string_cast.hpp>

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
  m_componentsLoader = std::make_unique<GameComponentsLoader>();
  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("player",
    [this] (GameObject& gameObject, const pugi::xml_node& data) {
    m_componentsLoader->loadPlayerData(gameObject, data);
  });

  auto resourceMgr = m_resourceManagementModule->getResourceManager();
  resourceMgr->loadResourcesMapFile("../resources/resources.xml");
  resourceMgr->loadResourcesMapFile("../resources/game/resources.xml");

  m_screenManager->registerScreen(std::make_shared<GameScreen>(m_inputModule,
    getGameApplicationSystemsGroup(),
    m_levelsManager));

  m_screenManager->registerScreen(std::make_shared<MainMenuScreen>(m_inputModule, m_gameConsole));

  std::shared_ptr deferredAccumulationPipeline = std::make_shared<GLShadersPipeline>(
    resourceMgr->getResourceFromInstance<ShaderResource>("deferred_accum_pass_vertex_shader")->getShader(),
    resourceMgr->getResourceFromInstance<ShaderResource>("deferred_accum_pass_fragment_shader")->getShader(),
    nullptr);

  m_renderingSystemsPipeline->setDeferredAccumulationShadersPipeline(deferredAccumulationPipeline);

  m_gameWorld->subscribeEventsListener<ScreenSwitchEvent>(this);

  m_screenManager->changeScreen(BaseGameScreen::getScreenName(GameScreenType::MainMenu));
}

void GameApplication::unload()
{
  m_componentsLoader.reset();
  m_gameWorld->unsubscribeEventsListener<ScreenSwitchEvent>(this);
}

EventProcessStatus GameApplication::receiveEvent(GameWorld* gameWorld, const ScreenSwitchEvent& event)
{
  ARG_UNUSED(gameWorld);
  ARG_UNUSED(event);

  if (event.newScreen->getName() == "Game") {
    m_engineGameSystems->getGameSystem<SkeletalAnimationSystem>()->setActive(true);
    m_engineGameSystems->getGameSystem<PhysicsSystem>()->setActive(true);

    m_renderingSystemsPipeline->setActive(true);
    m_gameApplicationSystems->setActive(true);
  }
  else {
    if (m_renderingSystemsPipeline->isActive()) {
      m_engineGameSystems->getGameSystem<SkeletalAnimationSystem>()->setActive(false);
      m_engineGameSystems->getGameSystem<PhysicsSystem>()->setActive(false);

      m_renderingSystemsPipeline->setActive(false);
      m_gameApplicationSystems->setActive(false);
    }
  }

  return EventProcessStatus::Processed;
}
