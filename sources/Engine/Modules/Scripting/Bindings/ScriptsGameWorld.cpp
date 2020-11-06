#include "precompiled.h"

#pragma hdrstop

#include "ScriptsGameWorld.h"

#include <utility>
#include "Modules/LevelsManagement/GameObjectsSpawnSystem.h"

ScriptsGameWorld::ScriptsGameWorld(std::shared_ptr<GameWorld> gameWorld)
  : m_gameWorld(std::move(gameWorld))
{

}

ScriptsGameWorld::~ScriptsGameWorld()
{

}

GameObject ScriptsGameWorld::findGameObject(const std::string& objectName)
{
  return m_gameWorld->findGameObject(objectName);
}

void ScriptsGameWorld::spawnGameObject(const std::string& spawnName,
  const glm::vec3& position,
  const glm::vec3& direction)
{
  m_gameWorld->emitEvent(SpawnGameObjectCommandEvent(spawnName, position, direction));
}

void ScriptsGameWorld::spawnUniqueGameObject(const std::string& spawnName,
  const std::string& objectName,
  const glm::vec3& position,
  const glm::vec3& direction)
{
  m_gameWorld->emitEvent(SpawnGameObjectCommandEvent(spawnName, position, direction, objectName));
}
