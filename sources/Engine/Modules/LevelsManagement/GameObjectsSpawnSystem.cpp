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
  const std::optional<std::string>& objectName)
  : m_objectSpawnName(std::move(objectSpawnName)),
    m_position(position),
    m_direction(direction),
    m_objectName(objectName)
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

  if (gameObject.hasComponent<RigidBodyComponent>()) {
    auto rigidBodyComponent = gameObject.getComponent<RigidBodyComponent>();

    Transform transform = transformComponent->getTransform();
    rigidBodyComponent->setTransform(transform);
  }

  getGameWorld()->emitEvent<AddObjectToSceneCommandEvent>(
    AddObjectToSceneCommandEvent{gameObject});

  spdlog::info("Spawn game object {} with name {} at position {}", event.getObjectSpawnName(),
    gameObject.getName(), glm::to_string(transformComponent->getTransform().getPosition()));

  return EventProcessStatus::Processed;
}
