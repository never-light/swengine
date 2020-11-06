#include "precompiled.h"

#pragma hdrstop

#include "GameObjectsSpawnSystem.h"

#include <utility>

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Physics/RigidBodyComponent.h"
#include "Modules/Math/MathUtils.h"

SpawnGameObjectCommandEvent::SpawnGameObjectCommandEvent(std::string objectSpawnName,
  const glm::vec3& position,
  const glm::vec3& direction,
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

const glm::vec3& SpawnGameObjectCommandEvent::getPosition() const
{
  return m_position;
}

const glm::vec3& SpawnGameObjectCommandEvent::getDirection() const
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
  transformComponent->getTransform().setPosition(event.getPosition());
  transformComponent->getTransform().setOrientation(
    MathUtils::forwardUpToQuat(event.getDirection(), MathUtils::AXIS_UP));

  if (gameObject.hasComponent<RigidBodyComponent>()) {
    auto rigidBodyComponent = gameObject.getComponent<RigidBodyComponent>();

    Transform transform = transformComponent->getTransform();
    rigidBodyComponent->setTransform(transform);
  }

  return EventProcessStatus::Processed;
}
