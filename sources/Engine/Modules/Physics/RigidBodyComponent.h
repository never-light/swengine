#pragma once

#include <memory>

#include "Modules/Graphics/GraphicsSystem/Transform.h"

#include "CollisionShapes.h"
#include "BaseBackend/RigidBodyComponentBackend.h"


struct RigidBodyComponent {
 public:
  RigidBodyComponent(float mass, std::shared_ptr<CollisionShape> collisionShape,
    std::shared_ptr<Transform> gameTransform);

  void setMass(float mass);
  [[nodiscard]] float getMass() const;

  void setTransform(const Transform& transform);

  void setLinearVelocity(const glm::vec3& velocity);
  [[nodiscard]] glm::vec3 getLinearVelocity() const;

 private:
  std::shared_ptr<RigidBodyComponentBackend> m_backendAdapter;

 private:
  friend class BulletPhysicsSystemBackend;
};