#include "Game.h"

#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>
#include <Engine/Modules/Graphics/GUI/GUISystem.h>
#include <Engine/Modules/Scripting/ScriptingSystem.h>

#include <Engine/Utility/files.h>

#include "Saving/SavingSystem.h"

#include <utility>

//#define START_WITH_FREE_CAMERA 1

static constexpr const char* START_LEVEL_ID = "cabinet";
//#define START_WITH_TMP_LEVEL "negan_anim"

Game::Game(std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup,
  std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<GraphicsScene> graphicsScene,
  std::shared_ptr<GUISystem> guiSystem,
  std::shared_ptr<ResourcesManager> resourceManager,
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
    m_freeCameraControlSystem(std::make_shared<FreeCameraControlSystem>(inputModule, graphicsScene, graphicsContext)),
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

#ifdef START_WITH_TMP_LEVEL
  m_resourceManager->loadResourcesMapFile(FileUtils::getLevelPath(START_LEVEL_ID) + "/../../../../tmp/" +
    std::string(START_WITH_TMP_LEVEL) + "/resources.xml");
#else
  m_resourceManager->loadResourcesMapFile(FileUtils::getLevelPath(START_LEVEL_ID) + "/resources.xml");
  m_resourceManager->loadResourcesMapFile(
    FileUtils::getGameResourcePath("resources_descs/game_dynamic.xml"));
#endif

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

void Game::unload()
{
  SW_ASSERT(m_isGameLoaded);

  m_gameModeSystems->removeGameSystem(m_inventoryControlSystem);
  m_gameModeSystems->removeGameSystem(m_interactiveObjectsControlSystem);
  m_gameModeSystems->removeGameSystem(m_questsSystem);
  m_gameModeSystems->removeGameSystem(m_infoportionsSystem);
  m_gameModeSystems->removeGameSystem(m_playerControlSystem);

  if (m_gameApplicationSystems->isConfigured()) {
    m_gameApplicationSystems->removeGameSystem(m_gameModeSystems);
  }

  m_graphicsScene->setActiveCamera(nullptr);
  m_preservedCameraControlSystem = nullptr;

  if (m_levelsManager->isLevelLoaded()) {
    m_levelsManager->unloadLevel();
    m_levelsManager->unloadSpawnLists();
  }

  // TODO: redesign levels loading/unloading logic, remove duplicated code like this
  for (GameObject gameObject : m_gameWorld->all()) {
    m_gameWorld->removeGameObject(gameObject);
  }

  m_isGameLoaded = false;
}

void Game::loadLevel(const std::string& levelName, bool isNewGame)
{
  SW_ASSERT(!m_isGameLoaded);
  SW_ASSERT(!m_levelsManager->isLevelLoaded());

  m_levelsManager->loadLevelsSpawnLists();

#ifdef START_WITH_TMP_LEVEL
  m_levelsManager->loadLevelSpawnList(std::string(START_LEVEL_ID) + "/../../../../tmp/" +
    std::string(START_WITH_TMP_LEVEL));
#else
  m_levelsManager->loadSpawnObjectsList("game_dynamic");
#endif

  m_gameWorld->emitEvent<ExecuteScriptParametricActionCommand>(
    ExecuteScriptParametricActionCommand::create("game.before_level_loading",
      std::make_tuple(isNewGame, levelName)));

  m_levelsManager->loadLevel(levelName);

  m_gameWorld->emitEvent<ExecuteScriptParametricActionCommand>(
    ExecuteScriptParametricActionCommand::create("game.on_level_loaded", std::make_tuple(isNewGame, levelName)));

  m_isGameLoaded = true;
}

void Game::createNewGame(const std::string& levelName)
{
  SW_ASSERT(!m_levelsManager->isLevelLoaded());
  loadLevel(levelName, true);
}

void Game::createLoadedGame(const std::string& levelName)
{
  SW_ASSERT(!m_levelsManager->isLevelLoaded());
  loadLevel(levelName, false);
}

void Game::setupGameState(bool isNewGame)
{
  m_gameApplicationSystems->addGameSystem(m_gameModeSystems);

  m_gameModeSystems->addGameSystem(m_inventoryControlSystem);
  m_gameModeSystems->addGameSystem(m_interactiveObjectsControlSystem);
  m_gameModeSystems->addGameSystem(m_questsSystem);
  m_gameModeSystems->addGameSystem(m_infoportionsSystem);

#if defined(START_WITH_FREE_CAMERA) && START_WITH_FREE_CAMERA == 1
  m_activeCameraControlSystem = m_freeCameraControlSystem;
#else
  m_activeCameraControlSystem = m_playerControlSystem;
#endif

  m_gameModeSystems->addGameSystem(m_activeCameraControlSystem);

  if (!m_isGamePreloaded) {
    m_dialoguesManager->loadFromFile("dialogues_general");
    m_infoportionsSystem->loadInfoportionsFromFile("infoportions_general");
    m_questsSystem->loadQuestsFromFile("quests_general");

    m_isGamePreloaded = true;
  }

#if defined(START_WITH_FREE_CAMERA) && START_WITH_FREE_CAMERA == 1
#else
  m_graphicsScene->setActiveCamera(m_gameWorld->findGameObject("player").getComponent<CameraComponent>()->getCamera());

  GameObject player = m_gameWorld->findGameObject("player");
  player.getComponent<CameraComponent>()->getCamera()->setAspectRatio(
    float(m_graphicsContext->getViewportWidth()) / float(m_graphicsContext->getViewportHeight()));
#endif

  auto environmentObject = m_gameWorld->findGameObject("environment");

  if (environmentObject.hasComponent<AudioSourceComponent>()) {
    environmentObject.getComponent<AudioSourceComponent>()->getSource().play();
  }

  if (isNewGame) {
    m_gameWorld->emitEvent<ExecuteScriptSimpleActionCommand>(
      ExecuteScriptSimpleActionCommand{"game.on_new_game"});
  }
}

bool Game::isLoaded() const
{
  return m_isGameLoaded;
}

