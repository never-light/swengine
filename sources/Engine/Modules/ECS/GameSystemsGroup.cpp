#include "precompiled.h"

#pragma hdrstop

#include "GameSystemsGroup.h"

#include <algorithm>

#include "GameSystem.h"
#include "GameWorld.h"

GameSystemsGroup::GameSystemsGroup(std::shared_ptr<GameWorld> gameWorld)
  : m_gameWorld(gameWorld)
{

}

GameSystemsGroup::~GameSystemsGroup()
{
  SW_ASSERT(!m_isConfigured);
}

void GameSystemsGroup::configure(GameWorld* gameWorld)
{
  SW_ASSERT(!m_isConfigured && !isActive());

  m_isConfigured = true;

  for (auto& system : m_gameSystems) {
    system->configure(gameWorld);
  }
}

void GameSystemsGroup::unconfigure(GameWorld* gameWorld)
{
  SW_ASSERT(m_isConfigured);

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->setActive(false);
    }

    system->unconfigure(gameWorld);
  }

  m_isConfigured = false;
}

void GameSystemsGroup::beforeRender(GameWorld* gameWorld)
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    system->beforeRender(gameWorld);
  }
}

void GameSystemsGroup::render(GameWorld* gameWorld)
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->render(gameWorld);
    }
  }
}

void GameSystemsGroup::afterRender(GameWorld* gameWorld)
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->afterRender(gameWorld);
    }
  }
}

void GameSystemsGroup::fixedUpdate(GameWorld* gameWorld, float delta)
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->fixedUpdate(gameWorld, delta);
    }
  }
}

void GameSystemsGroup::update(GameWorld* gameWorld, float delta)
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->update(gameWorld, delta);
    }
  }
}

void GameSystemsGroup::activate()
{
  for (auto& system : m_gameSystems) {
    SW_ASSERT(!system->isActive());
    system->setActive(true);
  }
}

void GameSystemsGroup::deactivate()
{
  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->setActive(false);
    }
  }
}

void GameSystemsGroup::addGameSystem(std::shared_ptr<GameSystem> system)
{
  system->m_gameWorld = m_gameWorld;

  m_gameSystems.push_back(system);

  if (m_isConfigured) {
    system->configure(m_gameWorld.lock().get());

    if (isActive()) {
      system->setActive(true);
    }
  }
}

void GameSystemsGroup::removeGameSystem(std::shared_ptr<GameSystem> system)
{
  if (m_isConfigured) {
    if (system->isActive()) {
      system->setActive(false);
    }

    system->unconfigure(m_gameWorld.lock().get());
  }

  system->m_gameWorld.reset();

  m_gameSystems.erase(std::remove(m_gameSystems.begin(), m_gameSystems.end(), system),
    m_gameSystems.end());
}

const std::vector<std::shared_ptr<GameSystem> >& GameSystemsGroup::getGameSystems() const
{
  return m_gameSystems;
}
