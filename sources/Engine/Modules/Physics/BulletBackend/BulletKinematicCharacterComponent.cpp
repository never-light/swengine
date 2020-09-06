#include "precompiled.h"

#pragma hdrstop

#include "BulletKinematicCharacterComponent.h"

#include <utility>
#include <BulletCollision/btBulletCollisionCommon.h>

#include "Modules/Math/MathUtils.h"
#include "BulletUtils.h"


BulletKinematicCharacterComponent::BulletKinematicCharacterComponent(std::shared_ptr<CollisionShape> collisionShape)
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
  m_kinematicController->setUpdateCallback(std::move(updateCallback));
}

