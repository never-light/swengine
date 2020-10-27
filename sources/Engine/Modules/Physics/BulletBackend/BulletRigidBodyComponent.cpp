#include "precompiled.h"

#pragma hdrstop

#include "Modules/Math/MathUtils.h"
#include "BulletRigidBodyComponent.h"

#include <utility>
#include "BulletUtils.h"

BulletRigidBodyComponent::BulletRigidBodyComponent(float mass, ResourceHandle<CollisionShape> collisionShape)
  : m_collisionShape(BulletUtils::convertCollisionShapeToBulletShape(*collisionShape))
{
  btTransform defaultTransform;
  defaultTransform.setIdentity();

  btCollisionShape* btShape = getBtCollisionShape();
  btVector3 localInertia(0, 0, 0);

  if (!MathUtils::isEqual(mass, 0.0f)) {
    btShape->calculateLocalInertia(mass, localInertia);
  }

  m_motionState = new BulletMotionState(defaultTransform);

  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, btShape, localInertia);
  m_rigidBodyInstance = new btRigidBody(rbInfo);
}

BulletRigidBodyComponent::~BulletRigidBodyComponent()
{
  delete m_rigidBodyInstance;
  delete m_motionState;
}

btCollisionShape* BulletRigidBodyComponent::getBtCollisionShape() const
{
  return m_collisionShape;
}

void BulletRigidBodyComponent::setMass(float mass)
{
  btVector3 localInertia(0, 0, 0);

  if (!MathUtils::isEqual(mass, 0.0f)) {
    getBtCollisionShape()->calculateLocalInertia(mass, localInertia);
  }

  m_rigidBodyInstance->setMassProps(mass, localInertia);
}

float BulletRigidBodyComponent::getMass() const
{
  return m_rigidBodyInstance->getMass();
}

void BulletRigidBodyComponent::setTransform(const Transform& transform)
{
  btTransform internalTransform = BulletUtils::internalTransformToBt(transform);

  m_motionState->setWorldTransform(internalTransform);
  m_rigidBodyInstance->setWorldTransform(internalTransform);
}

void BulletRigidBodyComponent::setLinearVelocity(const glm::vec3& velocity)
{
  m_rigidBodyInstance->setLinearVelocity(BulletUtils::glmVec3ToBt(velocity));
}

glm::vec3 BulletRigidBodyComponent::getLinearVelocity() const
{
  const btVector3& velocity = m_rigidBodyInstance->getLinearVelocity();

  return glm::vec3(velocity.x(), velocity.y(), velocity.z());
}

void BulletRigidBodyComponent::setAngularFactor(const glm::vec3& factor)
{
  m_rigidBodyInstance->setAngularFactor(BulletUtils::glmVec3ToBt(factor));
}

glm::vec3 BulletRigidBodyComponent::getAngularFactor() const
{
  return BulletUtils::btVec3ToGlm(m_rigidBodyInstance->getAngularFactor());
}

void BulletRigidBodyComponent::setLinearFactor(const glm::vec3& factor)
{
  m_rigidBodyInstance->setLinearFactor(BulletUtils::glmVec3ToBt(factor));
}

glm::vec3 BulletRigidBodyComponent::getLinearFactor() const
{
  return BulletUtils::btVec3ToGlm(m_rigidBodyInstance->getLinearFactor());
}

void BulletRigidBodyComponent::setUpdateCallback(std::function<void(const btTransform&)> updateCallback)
{
  auto* motionState = dynamic_cast<BulletMotionState*>(m_rigidBodyInstance->getMotionState());
  motionState->setUpdateCallback(std::move(updateCallback));
}
