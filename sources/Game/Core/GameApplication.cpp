#include "GameApplication.h"

#include <spdlog/spdlog.h>
#include <glm/gtx/string_cast.hpp>

#include <Engine/Exceptions/EngineRuntimeException.h>
#include <Engine/Modules/Graphics/Resources/SkeletonResourceManager.h>
#include <Engine/Utility/files.h>

#include "Game/Screens/GameScreen.h"
#include "Game/Screens/MainMenuScreen.h"
#include "Game/Screens/MainMenuSettingsScreen.h"

#include "Game/Inventory/InventoryUI.h"
#include "Game/Dynamic/DialoguesUI.h"

#include "Game/Saving/SavingSystem.h"

#include "Game/Game.h"

GameApplication::GameApplication(int argc, char* argv[])
  : BaseGameApplication(argc, argv, "Game")
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
  resourceMgr->loadResourcesMapFile("../resources/resources.xml");
  resourceMgr->loadResourcesMapFile("../resources/game/resources.xml");

  m_gameWorld->registerComponentBinderFactory<ActorComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<ActorComponent, ActorComponentBinder>>());
  m_gameWorld->registerComponentBinderFactory<InventoryComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<InventoryComponent, InventoryComponentBinder,
                                                                GameObjectsComponentsBinderInjectParameters::GameWorld>>(m_gameWorld));
  m_gameWorld->registerComponentBinderFactory<InventoryItemComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<InventoryItemComponent, InventoryItemComponentBinder,
                                                                GameObjectsComponentsBinderInjectParameters::ResourcesManager>>(resourceMgr));
  m_gameWorld->registerComponentBinderFactory<PlayerComponent>(
    std::make_shared<GameObjectsComponentsGenericBindersFactory<PlayerComponent, PlayerComponentBinder>>());

  m_componentsLoader = std::make_unique<GameComponentsLoader>(m_gameWorld, resourceMgr);
  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("player",
    [this](const pugi::xml_node& data) {
      return m_componentsLoader->loadPlayerData(data);
    });

  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("inventory_item",
    [this](const pugi::xml_node& data) {
      return m_componentsLoader->loadInventoryItemData(data);
    });

  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("inventory",
    [this](const pugi::xml_node& data) {
      return m_componentsLoader->loadInventoryData(data);
    });

  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("interactive",
    [this](const pugi::xml_node& data) {
      return m_componentsLoader->loadInteractiveData(data);
    });

  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("actor",
    [this](const pugi::xml_node& data) {
      return m_componentsLoader->loadActorData(data);
    });

  auto gameScreen = std::make_shared<GameScreen>(m_inputModule,
    getGameApplicationSystemsGroup(),
    m_levelsManager,
    m_graphicsScene,
    m_guiSystem,
    m_scriptingSystem->getScriptsExecutor());
  m_screenManager->registerScreen(gameScreen);

  auto mainMenuGUILayout = m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("screen_main_menu"));
  m_screenManager->registerScreen(std::make_shared<MainMenuScreen>(
    m_inputModule,
    mainMenuGUILayout,
    m_gameConsole));

  auto mainMenuSettingsGUILayout = m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("screen_main_menu_settings"));
  m_screenManager->registerScreen(std::make_shared<MainMenuSettingsScreen>(mainMenuSettingsGUILayout));

  GUIWidgetStylesheet commonStylesheet = m_guiSystem->loadStylesheet(
    FileUtils::getGUISchemePath("common.stylesheet"));
  m_screenManager->getCommonGUILayout()->applyStylesheet(commonStylesheet);

  std::shared_ptr deferredAccumulationPipeline = std::make_shared<GLShadersPipeline>(
    resourceMgr->getResource<GLShader>("deferred_accum_pass_vertex_shader"),
    resourceMgr->getResource<GLShader>("deferred_accum_pass_fragment_shader"),
    std::optional<ResourceHandle<GLShader>>());

  m_graphicsModule->getGraphicsContext()->setupDeferredAccumulationMaterial(deferredAccumulationPipeline);

  m_engineGameSystems->addGameSystem(std::make_shared<SavingSystem>(m_levelsManager,
    gameScreen->getGame()));

  m_gameWorld->subscribeEventsListener<AfterScreenSwitchEvent>(this);
  m_screenManager->changeScreen(BaseGameScreen::getScreenName(GameScreenType::MainMenu));
}

void GameApplication::unload()
{
  m_componentsLoader.reset();
  m_gameWorld->unsubscribeEventsListener<AfterScreenSwitchEvent>(this);
}

EventProcessStatus GameApplication::receiveEvent(const AfterScreenSwitchEvent& event)
{
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
