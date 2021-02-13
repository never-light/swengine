#include "precompiled.h"

#pragma hdrstop

#include "GameObjectsSpawnSystem.h"

#include <utility>
#include <spdlog/spdlog.h>
#include <glm/gtx/string_cast.hpp>

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/MeshRendererComponent.h"
#include "Modules/Graphics/GraphicsSystem/GraphicsSceneManagementSystem.h"
#include "Modules/Physics/RigidBodyComponent.h"
#include "Modules/Math/MathUtils.h"

SpawnGameObjectCommandEvent::SpawnGameObjectCommandEvent(std::string objectSpawnName,
  const std::optional<glm::vec3>& position,
  const std::optional<glm::vec3>& direction,
  std::optional<std::string> objectName,
  std::optional<std::string> levelName)
  : m_objectSpawnName(std::move(objectSpawnName)),
    m_position(position),
    m_direction(direction),
    m_objectName(std::move(objectName)),
    m_levelName(std::move(levelName))
{

}

const std::string& SpawnGameObjectCommandEvent::getObjectSpawnName() const
{
  return m_objectSpawnName;
}

const std::optional<glm::vec3>& SpawnGameObjectCommandEvent::getPosition() const
{
  return m_position;
}

const std::optional<glm::vec3>& SpawnGameObjectCommandEvent::getDirection() const
{
  return m_direction;
}

const std::optional<std::string>& SpawnGameObjectCommandEvent::getObjectName() const
{
  return m_objectName;
}

const std::optional<std::string>& SpawnGameObjectCommandEvent::getLevelName() const
{
  return m_levelName;
}

GameObjectsSpawnSystem::GameObjectsSpawnSystem(std::shared_ptr<LevelsManager> levelsManager)
  : m_levelsManager(std::move(levelsManager))
{

}

void GameObjectsSpawnSystem::activate()
{
  getGameWorld()->subscribeEventsListener<SpawnGameObjectCommandEvent>(this);
}

void GameObjectsSpawnSystem::deactivate()
{
  getGameWorld()->unsubscribeEventsListener<SpawnGameObjectCommandEvent>(this);
}

EventProcessStatus GameObjectsSpawnSystem::receiveEvent(const SpawnGameObjectCommandEvent& event)
{
  SW_ASSERT(m_levelsManager->isLevelLoaded());

  GameObject gameObject = m_levelsManager->getObjectsLoader().buildGameObject(
    event.getObjectSpawnName(), event.getObjectName());

  auto transformComponent = gameObject.getComponent<TransformComponent>();

  if (event.getPosition().has_value()) {
    transformComponent->getTransform().setPosition(event.getPosition().value());
  }

  if (event.getDirection().has_value()) {
    transformComponent->getTransform().setOrientation(
      MathUtils::forwardUpToQuat(event.getDirection().value(), MathUtils::AXIS_UP));
  }

  std::string levelId = event.getLevelName().value_or(m_levelsManager->getLoadedLevelName());
  transformComponent->setLevelId(levelId);

  // Spawn object as online object if it appear on the current level
  if (levelId == m_levelsManager->getLoadedLevelName()) {
    transformComponent->setOnlineMode(true);
  }
  else {
    SW_ASSERT(!transformComponent->isOnline());
  }

  // TODO: is it really needed here? Doesn't physics system update transform itself?
  if (gameObject.hasComponent<RigidBodyComponent>()) {
    auto rigidBodyComponent = gameObject.getComponent<RigidBodyComponent>();

    Transform transform = transformComponent->getTransform();
    rigidBodyComponent->setTransform(transform);
  }

  if (transformComponent->isOnline()) {
    getGameWorld()->emitEvent<AddObjectToSceneCommandEvent>(
      AddObjectToSceneCommandEvent{gameObject});
  }

  spdlog::info("Spawn game object: spawn_name {}, name {}, position {}, level {}, is_online {}",
    event.getObjectSpawnName(),
    gameObject.getName(),
    glm::to_string(transformComponent->getTransform().getPosition()),
    levelId,
    transformComponent->isOnline());

  return EventProcessStatus::Processed;
}
