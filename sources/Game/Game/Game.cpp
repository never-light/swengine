#include "Game.h"

#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>

Game::Game(std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<GameSystemsGroup> gameApplicationSystemsGroup,
  std::shared_ptr<InputModule> inputModule,
  std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
  std::shared_ptr<ResourceManager> resourceManager)
  : m_gameWorld(gameWorld),
    m_inputModule(inputModule),
    m_graphicsContext(graphicsContext),
    m_sharedGraphicsState(sharedGraphicsState),
    m_resourceManager(resourceManager),
    m_level(std::make_shared<GameLevel>(gameWorld, graphicsContext, resourceManager)),
    m_gameApplicationSystems(gameApplicationSystemsGroup),
    m_gameModeSystems(std::make_shared<GameSystemsGroup>(gameWorld)),
    m_playerControlSystem(std::make_shared<PlayerControlSystem>(inputModule))
{
  m_sharedGraphicsState->setActiveCamera(m_level->getPlayer()->getComponent<CameraComponent>()->getCamera());
  m_gameModeSystems->addGameSystem(m_playerControlSystem);
}

Game::~Game() = default;

void Game::activate()
{
  m_gameApplicationSystems->addGameSystem(m_gameModeSystems);
}

void Game::deactivate()
{
  m_gameApplicationSystems->removeGameSystem(m_gameModeSystems);
}

void Game::enterConsoleMode()
{
  if (m_gameModeSystems->getGameSystem<PlayerControlSystem>() != nullptr) {
    m_needRestorePlayerControl = true;
    m_gameModeSystems->removeGameSystem(m_playerControlSystem);
  }
}

void Game::leaveConsoleMode()
{
  if (m_needRestorePlayerControl) {
    m_gameModeSystems->addGameSystem(m_playerControlSystem);
  }
}