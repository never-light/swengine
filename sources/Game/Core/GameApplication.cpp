#include "GameApplication.h"

#include <spdlog/spdlog.h>
#include <glm/gtx/string_cast.hpp>

#include <Engine/Exceptions/EngineRuntimeException.h>
#include <Engine/Modules/Graphics/Resources/SkeletonResource.h>
#include <Engine/Utility/files.h>

#include "Game/Screens/GameScreen.h"
#include "Game/Screens/MainMenuScreen.h"

#include "Game/Inventory/InventoryUI.h"
#include "Game/Dynamic/DialoguesUI.h"

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
  resourceMgr->loadResourcesMapFile("../resources/resources.xml");
  resourceMgr->loadResourcesMapFile("../resources/game/resources.xml");

  m_componentsLoader = std::make_unique<GameComponentsLoader>(m_gameWorld, resourceMgr);
  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("player",
    [this](GameObject& gameObject, const pugi::xml_node& data) {
      m_componentsLoader->loadPlayerData(gameObject, data);
    });

  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("inventory_item",
    [this](GameObject& gameObject, const pugi::xml_node& data) {
      m_componentsLoader->loadInventoryItemData(gameObject, data);
    });

  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("inventory",
    [this](GameObject& gameObject, const pugi::xml_node& data) {
      m_componentsLoader->loadInventoryData(gameObject, data);
    });

  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("interactive",
    [this](GameObject& gameObject, const pugi::xml_node& data) {
      m_componentsLoader->loadInteractiveData(gameObject, data);
    });

  m_levelsManager->getObjectsLoader().registerGenericComponentLoader("actor",
    [this](GameObject& gameObject, const pugi::xml_node& data) {
      m_componentsLoader->loadActorData(gameObject, data);
    });

  m_guiSystem->getWidgetsLoader()->registerWidgetLoader("inventory_ui", [this](const pugi::xml_node& widgetData) {
    ARG_UNUSED(widgetData);
    return std::make_shared<InventoryUI>(m_gameWorld, m_inputModule);
  });

  m_dialoguesManager = std::make_shared<DialoguesManager>();
  m_dialoguesManager->loadFromFile("dialogues_general");

  m_guiSystem->getWidgetsLoader()->registerWidgetLoader("dialogues_ui", [this](const pugi::xml_node& widgetData) {
    ARG_UNUSED(widgetData);
    return std::make_shared<DialoguesUI>(m_gameWorld, m_dialoguesManager);
  });

  auto gameScreenDebugUILayout = m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("screen_game_debug"));

  PlayerUILayout playerUILayout;
  playerUILayout.playerUILayout = std::make_shared<GUILayout>();
  playerUILayout.playerUILayout->setSize(m_guiSystem->getActiveLayout()->getSize());

  playerUILayout.inventoryUI = std::dynamic_pointer_cast<InventoryUI>(m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("game_ui_inventory")));
  playerUILayout.interactionUI = std::dynamic_pointer_cast<GUILayout>(m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("game_ui_interaction")));
  playerUILayout.interactionUIText = std::dynamic_pointer_cast<GUIText>(
    playerUILayout.interactionUI->findChildByName("game_ui_interaction_action_text"));
  playerUILayout.dialoguesUI = std::dynamic_pointer_cast<DialoguesUI>(m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("game_ui_dialogues")));

  m_screenManager->registerScreen(
    std::make_shared<GameScreen>(m_inputModule,
      getGameApplicationSystemsGroup(),
      m_levelsManager,
      m_graphicsScene,
      gameScreenDebugUILayout,
      playerUILayout));

  auto mainMenuGUILayout = m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("screen_main_menu"));
  m_screenManager->registerScreen(std::make_shared<MainMenuScreen>(
    m_inputModule,
    mainMenuGUILayout,
    m_gameConsole));

  GUIWidgetStylesheet commonStylesheet = m_guiSystem->loadStylesheet(
    FileUtils::getGUISchemePath("common.stylesheet"));
  m_screenManager->getCommonGUILayout()->applyStylesheet(commonStylesheet);

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
