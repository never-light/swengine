#pragma once

#include <memory>

#include "Transform.h"

class TransformComponent {
 public:
  TransformComponent();

  [[nodiscard]] Transform* getTransform() const;

 private:
  std::unique_ptr<Transform> m_transform;
};

