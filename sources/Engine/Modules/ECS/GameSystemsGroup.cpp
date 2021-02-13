#include "precompiled.h"

#pragma hdrstop

#include "GameSystemsGroup.h"

#include <algorithm>

#include "GameWorld.h"

GameSystemsGroup::GameSystemsGroup() = default;

GameSystemsGroup::GameSystemsGroup(GameWorld* gameWorld)
  : GameSystem(gameWorld)
{

}

GameSystemsGroup::~GameSystemsGroup()
{
  SW_ASSERT(!m_isConfigured);
}

void GameSystemsGroup::configure()
{
  SW_ASSERT(!m_isConfigured && !isActive());

  m_isConfigured = true;

  for (auto& system : m_gameSystems) {
    system->configure();
  }
}

void GameSystemsGroup::unconfigure()
{
  SW_ASSERT(m_isConfigured);

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->setActive(false);
    }

    system->unconfigure();
  }

  m_isConfigured = false;
}

void GameSystemsGroup::beforeRender()
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    system->beforeRender();
  }
}

void GameSystemsGroup::render()
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->render();
    }
  }
}

void GameSystemsGroup::afterRender()
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->afterRender();
    }
  }
}

void GameSystemsGroup::fixedUpdate(float delta)
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->fixedUpdate(delta);
    }
  }
}

void GameSystemsGroup::update(float delta)
{
  if (!isActive()) {
    return;
  }

  for (auto& system : m_gameSystems) {
    if (system->isActive()) {
      system->update(delta);
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
  SW_ASSERT(m_gameWorld != nullptr && "Parent game system group should be added to other group");

  system->m_gameWorld = m_gameWorld;

  m_gameSystems.push_back(system);

  if (m_isConfigured) {
    system->configure();

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

    system->unconfigure();
  }

  system->m_gameWorld = nullptr;

  m_gameSystems.erase(std::remove(m_gameSystems.begin(), m_gameSystems.end(), system),
    m_gameSystems.end());
}

const std::vector<std::shared_ptr<GameSystem> >& GameSystemsGroup::getGameSystems() const
{
  return m_gameSystems;
}

bool GameSystemsGroup::isConfigured() const
{
  return m_isConfigured;
}
