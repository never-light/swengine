#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Physics/BaseBackend/KinematicCharacterComponentBackend.h"

#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "Modules/Physics/CollisionShapes.h"

#include "BulletKinematicCharacterController.h"

class BulletPhysicsSystemBackend;

class BulletKinematicCharacterComponent : public KinematicCharacterComponentBackend {
 public:
  explicit BulletKinematicCharacterComponent(ResourceHandle<CollisionShape> collisionShape);
  ~BulletKinematicCharacterComponent() override;

  void setTransform(const Transform& transform) override;

  void setPositionIncrement(const glm::vec3& increment) override;

  void jump(const glm::vec3& jumpVector) override;
  [[nodiscard]] bool isOnGround() const override;

  void setUpdateCallback(std::function<void(const btTransform&)> updateCallback);

  void setOriginOffset(const glm::vec3& offset) override;
  [[nodiscard]] glm::vec3 getOriginOffset() const override;

 private:
  btConvexShape* m_collisionShape = nullptr;
  BulletKinematicCharacterController* m_kinematicController = nullptr;
  btPairCachingGhostObject* m_ghostObject = nullptr;

  btVector3 m_originOffset = btVector3(0.0f, 0.0f, 0.0f);

 private:
  friend class BulletPhysicsSystemBackend;
};
