#include "Game.h"

#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>

#include <utility>

Game::Game(std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup,
  std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
  std::shared_ptr<ResourceManager> resourceManager,
  std::shared_ptr<LevelsManager> levelsManager,
  std::shared_ptr<GUILayout> gameUILayout,
  std::shared_ptr<InventoryUI> inventoryUILayout)
  : m_gameWorld(gameWorld),
    m_inputModule(inputModule),
    m_graphicsContext(graphicsContext),
    m_sharedGraphicsState(sharedGraphicsState),
    m_resourceManager(resourceManager),
    m_levelsManager(levelsManager),
    m_gameApplicationSystems(std::move(gameApplicationSystemsGroup)),
    m_gameModeSystems(std::make_shared<GameSystemsGroup>()),
    m_playerControlSystem(std::make_shared<PlayerControlSystem>(inputModule,
      sharedGraphicsState,
      gameUILayout,
      inventoryUILayout)),
    m_freeCameraControlSystem(std::make_shared<FreeCameraControlSystem>(inputModule, sharedGraphicsState)),
    m_inventoryControlSystem(std::make_shared<InventoryControlSystem>())
{
  m_gameApplicationSystems->addGameSystem(m_gameModeSystems);
  m_gameModeSystems->addGameSystem(m_inventoryControlSystem);

  m_level = std::make_shared<GameLevel>(gameWorld, graphicsContext, resourceManager, levelsManager);

  m_sharedGraphicsState->setActiveCamera(m_level->getPlayer().getComponent<CameraComponent>()->getCamera());
  m_activeCameraControlSystem = m_playerControlSystem;
  m_gameModeSystems->addGameSystem(m_playerControlSystem);
}

void Game::activate()
{
  m_gameWorld->subscribeEventsListener<GameConsoleCommandEvent>(this);
}

void Game::deactivate()
{
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

EventProcessStatus Game::receiveEvent(GameWorld* gameWorld, const GameConsoleCommandEvent& event)
{
  ARG_UNUSED(gameWorld);

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