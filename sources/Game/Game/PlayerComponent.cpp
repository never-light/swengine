#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(float playerHeight)
  : m_playerHeight(playerHeight)
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

void PlayerComponent::setDistanceToPlayer(float distance)
{
  m_distanceToPlayer = distance;
}

float PlayerComponent::getDistanceToPlayer() const
{
  return m_distanceToPlayer;
}

void PlayerComponent::setThirdPersonViewPitch(float pitch)
{
  m_thirdPersonViewPitch = pitch;
}

float PlayerComponent::getThirdPersonViewPitch() const
{
  return m_thirdPersonViewPitch;
}

void PlayerComponent::setThirdPersonViewYaw(float yaw)
{
  m_thirdPersonViewYaw = yaw;
}

float PlayerComponent::getThirdPersonViewYaw() const
{
  return m_thirdPersonViewYaw;
}

void PlayerComponent::increaseDistanceToPlayer(float delta)
{
  m_distanceToPlayer += delta;
}

void PlayerComponent::increaseThirdPersonViewPitch(float delta)
{
  m_thirdPersonViewPitch += delta;
}

void PlayerComponent::increaseThirdPersonViewYaw(float delta)
{
  m_thirdPersonViewYaw += delta;
}

float PlayerComponent::getPlayerHeight() const
{
  return m_playerHeight;
}
