#include "Game.h"

#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GUI/GUISystem.h>

#include <Engine/Utility/files.h>

#include <utility>

Game::Game(std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup,
  std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<GraphicsScene> graphicsScene,
  std::shared_ptr<GUISystem> guiSystem,
  std::shared_ptr<ResourceManager> resourceManager,
  std::shared_ptr<LevelsManager> levelsManager,
  std::shared_ptr<GUILayout> gameUILayout)
  : m_gameWorld(gameWorld),
    m_inputModule(inputModule),
    m_graphicsContext(graphicsContext),
    m_graphicsScene(graphicsScene),
    m_guiSystem(std::move(guiSystem)),
    m_resourceManager(resourceManager),
    m_levelsManager(levelsManager),
    m_gameApplicationSystems(std::move(gameApplicationSystemsGroup)),
    m_gameModeSystems(std::make_shared<GameSystemsGroup>()),
    m_freeCameraControlSystem(std::make_shared<FreeCameraControlSystem>(inputModule, graphicsScene)),
    m_inventoryControlSystem(std::make_shared<InventoryControlSystem>(levelsManager)),
    m_interactiveObjectsControlSystem(std::make_shared<InteractiveObjectsControlSystem>()),
    m_gameLogicConditionsManager(std::make_shared<GameLogicConditionsManager>(gameWorld)),
    m_questsStorage(std::make_shared<QuestsStorage>()),
    m_questsSystem(std::make_shared<QuestsSystem>(m_gameLogicConditionsManager, m_questsStorage)),
    m_infoportionsSystem(std::make_shared<InfoportionsSystem>()),
    m_dialoguesManager(std::make_shared<DialoguesManager>(m_gameLogicConditionsManager)),
    m_gameUILayout(std::move(gameUILayout))
{
  m_guiSystem->getWidgetsLoader()->registerWidgetLoader("inventory_ui",
    [this](const pugi::xml_node& widgetData) {
      ARG_UNUSED(widgetData);
      return std::make_shared<InventoryUI>(m_gameWorld, m_inputModule);
    });

  m_guiSystem->getWidgetsLoader()->registerWidgetLoader("dialogues_ui",
    [this](const pugi::xml_node& widgetData) {
      ARG_UNUSED(widgetData);
      return std::make_shared<DialoguesUI>(m_gameWorld, m_dialoguesManager, m_questsStorage);
    });

  m_dialoguesManager->loadFromFile("dialogues_general");

  m_playerUILayout.playerUILayout = std::make_shared<GUILayout>();
  m_playerUILayout.playerUILayout->setSize(m_guiSystem->getActiveLayout()->getSize());

  m_playerUILayout.hudLayout = std::dynamic_pointer_cast<GUILayout>(m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("game_ui_actor_hud")));
  m_playerUILayout.inventoryUI = std::dynamic_pointer_cast<InventoryUI>(m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("game_ui_inventory")));
  m_playerUILayout.interactionUI = std::dynamic_pointer_cast<GUILayout>(m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("game_ui_interaction")));
  m_playerUILayout.interactionUIText = std::dynamic_pointer_cast<GUIText>(
    m_playerUILayout.interactionUI->findChildByName("game_ui_interaction_action_text"));
  m_playerUILayout.dialoguesUI = std::dynamic_pointer_cast<DialoguesUI>(m_guiSystem->loadScheme(
    FileUtils::getGUISchemePath("game_ui_dialogues")));

  m_playerControlSystem = std::make_shared<PlayerControlSystem>(
    m_inputModule,
    m_graphicsScene,
    m_playerUILayout);

  m_gameApplicationSystems->addGameSystem(m_gameModeSystems);
  m_gameModeSystems->addGameSystem(m_inventoryControlSystem);
  m_gameModeSystems->addGameSystem(m_interactiveObjectsControlSystem);
  m_gameModeSystems->addGameSystem(m_questsSystem);
  m_gameModeSystems->addGameSystem(m_infoportionsSystem);

  m_infoportionsSystem->loadInfoportionsFromFile("infoportions_general");
  m_questsSystem->loadQuestsFromFile("quests_general");

  m_level = std::make_shared<GameLevel>(gameWorld, graphicsContext, resourceManager, levelsManager);

  m_graphicsScene->setActiveCamera(m_level->getPlayer().getComponent<CameraComponent>()->getCamera());
  m_activeCameraControlSystem = m_playerControlSystem;
  m_gameModeSystems->addGameSystem(m_playerControlSystem);
}

void Game::activate()
{
  m_gameWorld->subscribeEventsListener<GameConsoleCommandEvent>(this);
  m_gameUILayout->addChildWidget(m_playerUILayout.playerUILayout);
}

void Game::deactivate()
{
  m_gameUILayout->removeChildWidget(m_playerUILayout.playerUILayout);
  m_gameWorld->unsubscribeEventsListener<GameConsoleCommandEvent>(this);
}

void Game::enterConsoleMode()
{
  m_preservedCameraControlSystem = m_activeCameraControlSystem;

  if (m_activeCameraControlSystem == m_playerControlSystem) {
    m_gameModeSystems->removeGameSystem(m_playerControlSystem);
  }
  else if (m_activeCameraControlSystem == m_freeCameraControlSystem) {
    m_gameModeSystems->removeGameSystem(m_freeCameraControlSystem);
  }

  m_activeCameraControlSystem = nullptr;
}

void Game::leaveConsoleMode()
{
  if (m_preservedCameraControlSystem != nullptr) {
    m_gameModeSystems->addGameSystem(m_preservedCameraControlSystem);
    m_activeCameraControlSystem = m_preservedCameraControlSystem;
    m_preservedCameraControlSystem = nullptr;
  }
}

EventProcessStatus Game::receiveEvent(const GameConsoleCommandEvent& event)
{
  if (event.command == "free-camera") {
    m_preservedCameraControlSystem = m_freeCameraControlSystem;

    return EventProcessStatus::Processed;
  }
  else if (event.command == "player-camera") {
    m_preservedCameraControlSystem = m_playerControlSystem;

    return EventProcessStatus::Processed;
  }

  return EventProcessStatus::Skipped;
}
