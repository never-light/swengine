#pragma once

#include <glm/vec3.hpp>

struct CollisionShape {
 public:
  CollisionShape() = default;
  virtual ~CollisionShape() = default;
};

struct CollisionShapeSphere : public CollisionShape {
 public:
  explicit CollisionShapeSphere(float radius);
  ~CollisionShapeSphere() override = default;

  void setRadius(float radius);
  [[nodiscard]] float getRadius() const;

 private:
  float m_radius = 0.0f;
};

struct CollisionShapeBox : public CollisionShape {
 public:
  explicit CollisionShapeBox(const glm::vec3& halfExtents);
  ~CollisionShapeBox() override = default;

  void setHalfExtents(const glm::vec3& halfExtents);
  [[nodiscard]] glm::vec3 getHalfExtents() const;

 private:
  glm::vec3 m_halfExtents = glm::vec3(0.0f);
};

struct CollisionShapeCapsule : public CollisionShape {
 public:
  CollisionShapeCapsule(float radius, float height);
  ~CollisionShapeCapsule() override = default;

  void setRadius(float radius);
  [[nodiscard]] float getRadius() const;

  void setHeight(float height);
  [[nodiscard]] float getHeight() const;

 private:
  float m_radius = 0.0f;
  float m_height = 0.0f;
};
