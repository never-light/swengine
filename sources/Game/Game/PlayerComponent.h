#pragma once

#include <Engine/Modules/Graphics/GraphicsSystem/Camera.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Mesh.h>

class PlayerComponent {
 public:
  PlayerComponent();

  void setMovementSpeed(float speed);
  [[nodiscard]] float getMovementSpeed() const;

 private:
  float m_movementSpeed = 2.15f;
};

