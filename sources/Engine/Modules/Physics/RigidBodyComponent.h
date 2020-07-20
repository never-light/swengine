#pragma once

#include <memory>

#include "CollisionShapes.h"
#include "BulletBackend/BulletPhysicsBackend.h"

struct RigidBodyComponent {
 public:
  RigidBodyComponent(float mass, std::shared_ptr<CollisionShape> collisionShape);

  void setMass(float mass);
  float getMass() const;

  void setTransform(const Transform& transform);

  const CollisionShape& getCollisionShape() const;

 private:
  std::shared_ptr<RigidBodyComponentAdapter> m_backendAdapter;

 private:
  friend class BulletPhysicsAdapter;
};