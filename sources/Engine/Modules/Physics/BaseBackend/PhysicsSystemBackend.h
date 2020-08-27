#pragma once

#include "Modules/ECS/ECS.h"
#include "Modules/Math/geometry.h"

class PhysicsSystemBackend {
 public:
  explicit PhysicsSystemBackend() = default;
  virtual ~PhysicsSystemBackend() = default;

  virtual void configure() = 0;
  virtual void unconfigure() = 0;

  virtual void enableDebugDrawing(bool enable) = 0;
  virtual bool isDebugDrawingEnabled() = 0;

  virtual void setGravity(const glm::vec3& gravity) = 0;
  virtual glm::vec3 getGravity() const = 0;

  virtual void render() = 0;
  virtual void update(float delta) = 0;
};
