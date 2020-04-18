#include "Game.h"

#include <Engine/Modules/Graphics/GraphicsSystem/CameraComponent.h>

Game::Game(std::shared_ptr<GameWorld> gameWorld,
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
    m_gameSystems(std::make_shared<GameSystemsGroup>(gameWorld)),
    m_playerControlSystem(std::make_shared<PlayerControlSystem>(inputModule))
{
  m_sharedGraphicsState->setActiveCamera(m_level->getPlayer()->getComponent<CameraComponent>()->getCamera());
  m_gameSystems->addGameSystem(m_playerControlSystem);

}

Game::~Game()
{

}

void Game::activate()
{
  m_gameWorld->getGameSystemsGroup()->addGameSystem(m_gameSystems);
}

void Game::deactivate()
{
  m_gameWorld->getGameSystemsGroup()->removeGameSystem(m_gameSystems);
}
