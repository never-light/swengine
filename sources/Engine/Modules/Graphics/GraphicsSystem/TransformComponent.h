#pragma once

#include <memory>

#include "Transform.h"
#include "Modules/Math/geometry.h"

class TransformComponent {
 public:
  TransformComponent();

  [[nodiscard]] Transform& getTransform();
  [[nodiscard]] const Transform& getTransform() const;

  [[nodiscard]] std::shared_ptr<Transform> getTransformPtr() const;

  // TODO: move static mode settings to constructor parameters
  //  and do not allow to change it after component creation
  void setStaticMode(bool isStatic);
  [[nodiscard]] bool isStatic() const;

  void updateBounds(const glm::mat4& transformation);
  void updateBounds(const glm::vec3& origin, const glm::quat& orientation);

  [[nodiscard]] const AABB& getBoundingBox() const;
  [[nodiscard]] const Sphere& getBoundingSphere() const;

  void setBounds(const AABB& bounds);

 private:
  std::shared_ptr<Transform> m_transform;

  bool m_isStatic = false;

  AABB m_boundingBox;
  Sphere m_boundingSphere;

  AABB m_originalBounds;
};

