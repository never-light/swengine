#include "precompiled.h"

#pragma hdrstop

#include "RigidBodyComponent.h"

#include <utility>
#include "PhysicsBackendFactory.h"

RigidBodyComponent::RigidBodyComponent(float mass, std::shared_ptr<CollisionShape> collisionShape)
  : m_backend(PhysicsBackendFactory::createRigidBodyComponent(mass, std::move(collisionShape)))
{

}

void RigidBodyComponent::setMass(float mass)
{
  m_backend->setMass(mass);
}

float RigidBodyComponent::getMass() const
{
  return m_backend->getMass();
}

void RigidBodyComponent::setTransform(const Transform& transform)
{
  m_backend->setTransform(transform);
}

void RigidBodyComponent::setLinearVelocity(const glm::vec3& velocity)
{
  m_backend->setLinearVelocity(velocity);
}

glm::vec3 RigidBodyComponent::getLinearVelocity() const
{
  return m_backend->getLinearVelocity();
}

void RigidBodyComponent::setCollisionCallback(CollisionCallback callback)
{
  m_collisionCallback = std::move(callback);
}

CollisionCallback RigidBodyComponent::getCollisionCallback() const
{
  return m_collisionCallback;
}

const RigidBodyComponentBackend& RigidBodyComponent::getBackend() const
{
  return *m_backend;
}

void RigidBodyComponent::resetBackend()
{
  m_backend = nullptr;
}

void RigidBodyComponent::setAngularFactor(const glm::vec3& factor)
{
  m_backend->setAngularFactor(factor);
}

[[nodiscard]] glm::vec3 RigidBodyComponent::getAngularFactor() const
{
  return m_backend->getAngularFactor();
}

void RigidBodyComponent::setLinearFactor(const glm::vec3& factor)
{
  m_backend->setLinearFactor(factor);
}

RigidBodyComponentBackend& RigidBodyComponent::getBackend()
{
  return *m_backend;
}

[[nodiscard]] glm::vec3 RigidBodyComponent::getLinearFactor() const
{
  return m_backend->getLinearFactor();
}
