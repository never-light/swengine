#include "precompiled.h"

#pragma hdrstop

#include "BulletKinematicCharacterComponent.h"

#include <utility>
#include <BulletCollision/btBulletCollisionCommon.h>

#include "Modules/Math/MathUtils.h"
#include "BulletUtils.h"

BulletKinematicCharacterComponent::BulletKinematicCharacterComponent(ResourceHandle<CollisionShape> collisionShape)
  : m_collisionShape(dynamic_cast<btConvexShape*>(BulletUtils::convertCollisionShapeToBulletShape(*collisionShape)))
{
  SW_ASSERT(m_collisionShape != nullptr && "Bullet kinematic character should have convex collision shape");

  m_ghostObject = new btPairCachingGhostObject();
  m_ghostObject->setCollisionShape(m_collisionShape);
  m_ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

  m_kinematicController = new BulletKinematicCharacterController(m_ghostObject, m_collisionShape);
}

BulletKinematicCharacterComponent::~BulletKinematicCharacterComponent()
{
  delete m_kinematicController;
  delete m_ghostObject;
}

void BulletKinematicCharacterComponent::setTransform(const Transform& transform)
{
  btTransform internalTransform = BulletUtils::internalTransformToBt(transform);
  internalTransform.setOrigin(internalTransform.getOrigin() + m_originOffset);

  m_ghostObject->setWorldTransform(internalTransform);
}

void BulletKinematicCharacterComponent::setPositionIncrement(const glm::vec3& increment)
{
  m_kinematicController->setWalkDirection(BulletUtils::glmVec3ToBt(increment));
}

void BulletKinematicCharacterComponent::jump(const glm::vec3& jumpVector)
{
  m_kinematicController->jump(BulletUtils::glmVec3ToBt(jumpVector));
}

bool BulletKinematicCharacterComponent::isOnGround() const
{
  return m_kinematicController->onGround();
}

void BulletKinematicCharacterComponent::setUpdateCallback(std::function<void(const btTransform&)> updateCallback)
{
  m_kinematicController->setUpdateCallback([updateCallback, this](const btTransform& transform) {
    btTransform shiftedTransform = transform;
    shiftedTransform.setOrigin(shiftedTransform.getOrigin() - this->m_originOffset);

    updateCallback(shiftedTransform);
  });
}

void BulletKinematicCharacterComponent::setOriginOffset(const glm::vec3& offset)
{
  m_originOffset = BulletUtils::glmVec3ToBt(offset);
}

glm::vec3 BulletKinematicCharacterComponent::getOriginOffset() const
{
  return BulletUtils::btVec3ToGlm(m_originOffset);
}

void BulletKinematicCharacterComponent::enableSimulation(bool enable)
{
  m_ghostObject->setActivationState(((enable) ? ACTIVE_TAG : DISABLE_SIMULATION));
}

bool BulletKinematicCharacterComponent::isSimulationEnabled() const
{
  return m_ghostObject->getActivationState() != DISABLE_SIMULATION;
}

