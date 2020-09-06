#include "precompiled.h"

#pragma hdrstop

#include "KinematicCharacterComponent.h"

#include <utility>
#include "PhysicsBackendFactory.h"

KinematicCharacterComponent::KinematicCharacterComponent(std::shared_ptr<CollisionShape> collisionShape)
  : m_backend(PhysicsBackendFactory::createKinematicCharacterComponent(std::move(collisionShape)))
{

}

void KinematicCharacterComponent::setTransform(const Transform& transform)
{
  m_backend->setTransform(transform);
}

void KinematicCharacterComponent::setPositionIncrement(const glm::vec3& increment)
{
  m_backend->setPositionIncrement(increment);
}

void KinematicCharacterComponent::jump(const glm::vec3& jumpVector)
{
  m_backend->jump(jumpVector);
}

bool KinematicCharacterComponent::isOnGround() const
{
  return m_backend->isOnGround();
}

void KinematicCharacterComponent::setCollisionCallback(CollisionCallback callback)
{
  m_collisionCallback = std::move(callback);
}

CollisionCallback KinematicCharacterComponent::getCollisionCallback() const
{
  return m_collisionCallback;
}

const KinematicCharacterComponentBackend& KinematicCharacterComponent::getBackend() const
{
  return *m_backend;
}

void KinematicCharacterComponent::resetBackend()
{
  m_backend = nullptr;
}
