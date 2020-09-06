#pragma once

#include <memory>

#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "BaseBackend/RigidBodyComponentBackend.h"

#include "CollisionShapes.h"
#include "PhysicsCollisions.h"

struct RigidBodyComponent {
 public:
  RigidBodyComponent(float mass, std::shared_ptr<CollisionShape> collisionShape);

  void setMass(float mass);
  [[nodiscard]] float getMass() const;

  void setTransform(const Transform& transform);

  void setLinearVelocity(const glm::vec3& velocity);
  [[nodiscard]] glm::vec3 getLinearVelocity() const;

  void setCollisionCallback(CollisionCallback callback);
  [[nodiscard]] CollisionCallback getCollisionCallback() const;

  void setAngularFactor(const glm::vec3& factor);
  [[nodiscard]] glm::vec3 getAngularFactor() const;

  void setLinearFactor(const glm::vec3& factor);
  [[nodiscard]] glm::vec3 getLinearFactor() const;

  [[nodiscard]] const RigidBodyComponentBackend& getBackend() const;
  void resetBackend();

 private:
  std::shared_ptr<RigidBodyComponentBackend> m_backend;
  CollisionCallback m_collisionCallback;
};