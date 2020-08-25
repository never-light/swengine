#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>

#include "Modules/Physics/BaseBackend/RigidBodyComponentBackend.h"

#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "Modules/Physics/CollisionShapes.h"

#include "BulletMotionState.h"
#include "BulletPhysicsSystemBackend.h"

class BulletRigidBodyComponent : public RigidBodyComponentBackend {
 public:
  BulletRigidBodyComponent(float mass, std::shared_ptr<CollisionShape> collisionShape,
    std::shared_ptr<Transform> gameTransform);
  ~BulletRigidBodyComponent() override;

  void setMass(float mass) override;
  [[nodiscard]] float getMass() const override;

  [[nodiscard]] btCollisionShape* getBtCollisionShape() const;

  void setTransform(const Transform& transform) override;

  void setLinearVelocity(const glm::vec3& velocity) override;
  [[nodiscard]] glm::vec3 getLinearVelocity() const override;

 private:
  [[nodiscard]] static btCollisionShape* convertCollisionShapeToBulletShape(const CollisionShape& shape);

 private:
  btCollisionShape* m_collisionShape = nullptr;

  BulletMotionState* m_motionState = nullptr;
  btRigidBody* m_rigidBodyInstance = nullptr;

 private:
  friend class BulletPhysicsSystemBackend;
};
