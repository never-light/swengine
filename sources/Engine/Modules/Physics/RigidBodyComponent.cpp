#include "precompiled.h"

#pragma hdrstop

#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(float mass, std::shared_ptr<CollisionShape> collisionShape,
  std::shared_ptr<Transform> gameTransform)
  : m_backendAdapter(std::make_shared<BulletRigidBodyComponentAdapter>(mass, collisionShape, gameTransform))
{

}

void RigidBodyComponent::setMass(float mass)
{
  m_backendAdapter->setMass(mass);
}

float RigidBodyComponent::getMass() const
{
  return m_backendAdapter->getMass();
}

void RigidBodyComponent::setTransform(const Transform& transform)
{
  m_backendAdapter->setTransform(transform);
}

void RigidBodyComponent::setLinearVelocity(const glm::vec3& velocity)
{
  m_backendAdapter->setLinearVelocity(velocity);
}

glm::vec3 RigidBodyComponent::getLinearVelocity() const
{
  return m_backendAdapter->getLinearVelocity();
}
