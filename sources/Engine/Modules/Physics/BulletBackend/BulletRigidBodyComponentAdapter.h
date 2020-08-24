#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>

#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "Modules/Physics/CollisionShapes.h"

#include "BulletMotionState.h"

class BulletRigidBodyComponentAdapter {
 public:
  BulletRigidBodyComponentAdapter(float mass, std::shared_ptr<CollisionShape> collisionShape,
    std::shared_ptr<Transform> gameTransform);
  ~BulletRigidBodyComponentAdapter();

  void setMass(float mass);
  [[nodiscard]] float getMass() const;

  [[nodiscard]] btCollisionShape* getBtCollisionShape() const;

  void setTransform(const Transform& transform);

  void setLinearVelocity(const glm::vec3& velocity);
  glm::vec3 getLinearVelocity() const;

 private:
  [[nodiscard]] static btCollisionShape* convertCollisionShapeToBulletShape(const CollisionShape& shape);

 private:
  btCollisionShape* m_collisionShape = nullptr;

  BulletMotionState* m_motionState = nullptr;
  btRigidBody* m_rigidBodyInstance = nullptr;

 private:
  friend class BulletPhysicsAdapter;
};
