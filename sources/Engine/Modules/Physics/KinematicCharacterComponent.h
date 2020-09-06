#pragma once

#include <memory>

#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "BaseBackend/KinematicCharacterComponentBackend.h"

#include "CollisionShapes.h"
#include "PhysicsCollisions.h"

struct KinematicCharacterComponent {
 public:
  explicit KinematicCharacterComponent(std::shared_ptr<CollisionShape> collisionShape);

  void setTransform(const Transform& transform);

  void setPositionIncrement(const glm::vec3& increment);

  void jump(const glm::vec3& jumpVector);
  [[nodiscard]] bool isOnGround() const;

  void setOriginOffset(const glm::vec3& offset);
  [[nodiscard]] glm::vec3 getOriginOffset() const;

  void setCollisionCallback(CollisionCallback callback);
  [[nodiscard]] CollisionCallback getCollisionCallback() const;

  [[nodiscard]] const KinematicCharacterComponentBackend& getBackend() const;
  [[nodiscard]] KinematicCharacterComponentBackend& getBackend();

  void resetBackend();

 private:
  std::shared_ptr<KinematicCharacterComponentBackend> m_backend;
  CollisionCallback m_collisionCallback;
};