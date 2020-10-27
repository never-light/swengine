#pragma once

#include <variant>
#include <glm/vec3.hpp>

#include "Modules/ResourceManagement/ResourcesManagement.h"

struct CollisionShapeSphere {
 public:
  CollisionShapeSphere() = default;
  explicit CollisionShapeSphere(float radius);
  ~CollisionShapeSphere() = default;

  void setRadius(float radius);
  [[nodiscard]] float getRadius() const;

 private:
  float m_radius = 0.0f;
};

struct CollisionShapeBox {
 public:
  CollisionShapeBox() = default;
  explicit CollisionShapeBox(const glm::vec3& halfExtents);
  ~CollisionShapeBox() = default;

  void setHalfExtents(const glm::vec3& halfExtents);
  [[nodiscard]] glm::vec3 getHalfExtents() const;

 private:
  glm::vec3 m_halfExtents = glm::vec3(0.0f);
};

struct CollisionShapeCapsule {
 public:
  CollisionShapeCapsule() = default;
  CollisionShapeCapsule(float radius, float height);
  ~CollisionShapeCapsule() = default;

  void setRadius(float radius);
  [[nodiscard]] float getRadius() const;

  void setHeight(float height);
  [[nodiscard]] float getHeight() const;

 private:
  float m_radius = 0.0f;
  float m_height = 0.0f;
};

struct CollisionShapeTriangleMesh {
 public:
  CollisionShapeTriangleMesh() = default;
  explicit CollisionShapeTriangleMesh(std::vector<glm::vec3> vertices);
  ~CollisionShapeTriangleMesh() = default;

  void setVertices(const std::vector<glm::vec3>& vertices);
  [[nodiscard]] const std::vector<glm::vec3>& getVertices() const;

 private:
  std::vector<glm::vec3> m_vertices;
};

struct CollisionShapeCompound;

using CollisionShapeData = std::variant<CollisionShapeSphere, CollisionShapeCapsule,
                                        CollisionShapeBox, CollisionShapeTriangleMesh, CollisionShapeCompound>;

struct CollisionShapeCompoundChild;

struct CollisionShapeCompound {
 public:
  explicit CollisionShapeCompound(std::vector<CollisionShapeCompoundChild> children);
  ~CollisionShapeCompound() = default;

  void setChildren(const std::vector<CollisionShapeCompoundChild>& children);
  [[nodiscard]] const std::vector<CollisionShapeCompoundChild>& getChildren() const;

 private:
  std::vector<CollisionShapeCompoundChild> m_children;
};

struct CollisionShapeCompoundChild {
  CollisionShapeData shape;
  glm::vec3 origin;
};

class CollisionShape : public Resource {
 public:
  CollisionShape() = default;
  explicit CollisionShape(const CollisionShapeData& shapeData);

  ~CollisionShape() override = default;

  CollisionShape& operator=(const CollisionShape&) = default;

  void setShapeData(const CollisionShapeData& shapeData);
  [[nodiscard]] const CollisionShapeData& getShapeData() const;

 private:
  CollisionShapeData m_shapeData;
};