#pragma once

#include "Modules/ECS/ECS.h"
#include "Modules/Math/geometry.h"
#include "Modules/Graphics/GraphicsSystem/Transform.h"

class RigidBodyComponentBackend {
 public:
  RigidBodyComponentBackend() = default;
  virtual ~RigidBodyComponentBackend() = default;

  virtual void setMass(float mass) = 0;
  [[nodiscard]] virtual float getMass() const = 0;

  virtual void setTransform(const Transform& transform) = 0;

  virtual void setLinearVelocity(const glm::vec3& velocity) = 0;
  [[nodiscard]] virtual glm::vec3 getLinearVelocity() const = 0;

  virtual void setAngularFactor(const glm::vec3& factor) = 0;
  [[nodiscard]] virtual glm::vec3 getAngularFactor() const = 0;

  virtual void setLinearFactor(const glm::vec3& factor) = 0;
  [[nodiscard]] virtual glm::vec3 getLinearFactor() const = 0;
};
