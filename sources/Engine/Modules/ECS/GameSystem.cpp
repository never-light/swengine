#include "precompiled.h"

#pragma hdrstop

#include "GameSystem.h"

GameSystem::GameSystem() = default;

GameSystem::GameSystem(GameWorld* gameWorld)
  : m_gameWorld(gameWorld)
{

}

GameSystem::~GameSystem() = default;

void GameSystem::setActive(bool isActive)
{
  SW_ASSERT(isActive != m_isActive);

  if (isActive) {
    m_isActive = true;
    activate();
  }
  else {
    m_isActive = false;
    deactivate();
  }
}

bool GameSystem::isActive() const
{
  return m_isActive;
}

void GameSystem::activate()
{
}

void GameSystem::deactivate()
{
}

void GameSystem::fixedUpdate(float delta)
{
  ARG_UNUSED(delta);
}

void GameSystem::update(float delta)
{
  ARG_UNUSED(delta);
}

void GameSystem::render()
{

}

void GameSystem::beforeRender()
{

}

void GameSystem::afterRender()
{

}

void GameSystem::configure()
{

}

void GameSystem::unconfigure()
{

}
