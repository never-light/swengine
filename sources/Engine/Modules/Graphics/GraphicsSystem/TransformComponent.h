#pragma once

#include <memory>

#include "Transform.h"

class TransformComponent {
 public:
  TransformComponent();

  [[nodiscard]] Transform& getTransform();
  [[nodiscard]] const Transform& getTransform() const;

 private:
  std::shared_ptr<Transform> m_transform;
};

