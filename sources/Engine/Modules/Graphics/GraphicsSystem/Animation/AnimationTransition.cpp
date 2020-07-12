#include "precompiled.h"

#pragma hdrstop

#include "AnimationTransition.h"


AnimationTransition::AnimationTransition(AnimationStatesTransitionType type, float duration, bool isActive)
  : m_isActive(isActive),
  m_type(type),
  m_duration(duration)
{

}

bool AnimationTransition::isActive() const
{
  return m_isActive;
}

void AnimationTransition::setType(AnimationStatesTransitionType type)
{
  m_type = type;
}

AnimationStatesTransitionType AnimationTransition::getType() const
{
  return m_type;
}

void AnimationTransition::setDuration(float duration)
{
  m_duration = duration;
}

float AnimationTransition::getDuration() const
{
  return m_duration;
}

void AnimationTransition::activate()
{
  m_isActive = true;
}

void AnimationTransition::deactivate()
{
  m_isActive = false;
}

void AnimationTransition::setCondition(std::shared_ptr<ConditionalNode> condition)
{
  m_condition = std::move(condition);
}

std::shared_ptr<ConditionalNode> AnimationTransition::getCondition() const
{
  return m_condition;
}
