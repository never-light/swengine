#include "PlayerComponent.h"

PlayerComponent::PlayerComponent()
{

}

void PlayerComponent::setMovementSpeed(float speed)
{
    m_movementSpeed = speed;
}

float PlayerComponent::getMovementSpeed() const
{
    return m_movementSpeed;
}
