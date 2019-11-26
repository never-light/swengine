#pragma once

#include <Engine/Modules/Graphics/GraphicsSystem/Camera.h>

class PlayerComponent
{
public:
    PlayerComponent();

    void setMovementSpeed(float speed);
    float getMovementSpeed() const;

private:
    float m_movementSpeed = 0.15f;
};

