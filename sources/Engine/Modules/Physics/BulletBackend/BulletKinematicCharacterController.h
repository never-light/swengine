#pragma once

#include <functional>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

class BulletKinematicCharacterController : public btKinematicCharacterController {
 public:
  BulletKinematicCharacterController(btPairCachingGhostObject* ghostObject,
    btConvexShape * convexShape,
    btScalar stepHeight = 0.05f,
    const btVector3& up = btVector3(0.0f, 1.0f, 0.0f));

  ~BulletKinematicCharacterController() override = default;

  void updateAction(btCollisionWorld * collisionWorld, btScalar deltaTime) override;

  void setUpdateCallback(std::function<void(const btTransform&)> updateCallback);

 private:
  std::function<void(const btTransform&)> m_updateCallback;
};