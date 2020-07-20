#pragma once

#include <memory>
#include <btBulletDynamicsCommon.h>

#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "Modules/Physics/CollisionShapes.h"

class BulletRigidBodyComponentAdapter {
 public:
  BulletRigidBodyComponentAdapter(float mass, std::shared_ptr<CollisionShape> collisionShape);
  ~BulletRigidBodyComponentAdapter();

  void setMass(float mass);
  float getMass() const;

  const CollisionShape& getCollisionShape() const;

  btCollisionShape* getBtCollisionShape() const;

  void setTransform(const Transform& transform);

 private:
  std::shared_ptr<CollisionShape> m_collisionShape = nullptr;

  btDefaultMotionState* m_motionState = nullptr;
  btRigidBody* m_rigidBodyInstance = nullptr;

 private:
  friend class BulletPhysicsAdapter;
};
