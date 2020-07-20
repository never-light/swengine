#include "precompiled.h"

#pragma hdrstop

#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(float mass, std::shared_ptr<CollisionShape> collisionShape)
  : m_backendAdapter(std::make_shared<BulletRigidBodyComponentAdapter>(mass, collisionShape))
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

const CollisionShape& RigidBodyComponent::getCollisionShape() const
{
  return m_backendAdapter->getCollisionShape();
}

void RigidBodyComponent::setTransform(const Transform& transform)
{
  m_backendAdapter->setTransform(transform);
}
