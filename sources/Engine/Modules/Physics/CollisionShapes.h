#pragma once

#include <glm/vec3.hpp>

struct CollisionShape {
 public:
  CollisionShape() = default;
  virtual ~CollisionShape() = default;
};

struct CollisionShapeSphere : public CollisionShape {
 public:
  CollisionShapeSphere() = default;
  ~CollisionShapeSphere() override = default;

  [[nodiscard]] virtual float getRadius() const = 0;
};

struct CollisionShapeBox : public CollisionShape {
 public:
  CollisionShapeBox() = default;
  ~CollisionShapeBox() override = default;

  [[nodiscard]] virtual glm::vec3 getHalfExtents() const = 0;
};

struct CollisionShapeCapsule : public CollisionShape {
 public:
  CollisionShapeCapsule() = default;
  ~CollisionShapeCapsule() override = default;

  [[nodiscard]] virtual float getRadius() const = 0;
  [[nodiscard]] virtual float getHeight() const = 0;
};
