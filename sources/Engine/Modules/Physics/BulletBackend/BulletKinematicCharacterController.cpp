#include "precompiled.h"

#pragma hdrstop

#include "BulletKinematicCharacterController.h"

#include <utility>

BulletKinematicCharacterController::BulletKinematicCharacterController(btPairCachingGhostObject* ghostObject,
  btConvexShape* convexShape,
  btScalar stepHeight,
  const btVector3& up) : btKinematicCharacterController(ghostObject, convexShape, stepHeight, up)
{

}

void BulletKinematicCharacterController::updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime)
{
  btKinematicCharacterController::updateAction(collisionWorld, deltaTime);

  if (m_updateCallback != nullptr) {
    m_updateCallback(m_ghostObject->getWorldTransform());
  }
}

void BulletKinematicCharacterController::setUpdateCallback(std::function<void(const btTransform&)> updateCallback)
{
  m_updateCallback = std::move(updateCallback);
}
