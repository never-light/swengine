#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>

#include "Modules/ResourceManagement/ResourcesManagement.h"

#include "Modules/Physics/BaseBackend/RigidBodyComponentBackend.h"

#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "Modules/Physics/CollisionShapes.h"

#include "BulletMotionState.h"

class BulletPhysicsSystemBackend;

class BulletRigidBodyComponent : public RigidBodyComponentBackend {
 public:
  BulletRigidBodyComponent(float mass, ResourceHandle<CollisionShape> collisionShape);
  ~BulletRigidBodyComponent() override;

  void setMass(float mass) override;
  [[nodiscard]] float getMass() const override;

  [[nodiscard]] btCollisionShape* getBtCollisionShape() const;

  void setTransform(const Transform& transform) override;

  void setLinearVelocity(const glm::vec3& velocity) override;
  [[nodiscard]] glm::vec3 getLinearVelocity() const override;

  void setAngularFactor(const glm::vec3& factor) override;
  [[nodiscard]] glm::vec3 getAngularFactor() const override;

  void setLinearFactor(const glm::vec3& factor) override;
  [[nodiscard]] glm::vec3 getLinearFactor() const override;

  void setUpdateCallback(std::function<void(const btTransform&)> updateCallback);

 private:
  btCollisionShape* m_collisionShape = nullptr;

  BulletMotionState* m_motionState = nullptr;
  btRigidBody* m_rigidBodyInstance = nullptr;

 private:
  friend class BulletPhysicsSystemBackend;
};
