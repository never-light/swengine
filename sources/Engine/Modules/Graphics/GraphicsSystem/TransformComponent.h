#pragma once

#include <memory>

#include "Transform.h"

class TransformComponent {
 public:
  TransformComponent();

  [[nodiscard]] Transform& getTransform();
  [[nodiscard]] const Transform& getTransform() const;

  [[nodiscard]] std::shared_ptr<Transform> getTransformPtr() const;

  void setStaticMode(bool isStatic);
  [[nodiscard]] bool isStatic() const;

 private:
  std::shared_ptr<Transform> m_transform;

  bool m_isStatic = false;
};

