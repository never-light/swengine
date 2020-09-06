#pragma once

#include "Modules/ECS/ECS.h"
#include "Modules/Math/geometry.h"
#include "Modules/Graphics/GraphicsSystem/Transform.h"

class KinematicCharacterComponentBackend {
 public:
  KinematicCharacterComponentBackend() = default;
  virtual ~KinematicCharacterComponentBackend() = default;

  virtual void setTransform(const Transform& transform) = 0;

  virtual void setPositionIncrement(const glm::vec3& increment) = 0;

  virtual void jump(const glm::vec3& jumpVector) = 0;
  [[nodiscard]] virtual bool isOnGround() const = 0;

  virtual void setOriginOffset(const glm::vec3& offset) = 0;
  [[nodiscard]] virtual glm::vec3 getOriginOffset() const = 0;
};
