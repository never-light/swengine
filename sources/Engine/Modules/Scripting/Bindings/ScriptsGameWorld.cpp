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
  const std::optional<std::string>& objectName,
  const std::optional<glm::vec3>& position,
  const std::optional<glm::vec3>& direction,
  const std::optional<std::string> levelName)
{
  m_gameWorld->emitEvent(SpawnGameObjectCommandEvent(spawnName, position, direction, objectName, levelName));
}
