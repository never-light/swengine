#include "precompiled.h"

#pragma hdrstop

#include "AnimationStatesMachineVariables.h"

AnimationStatesMachineVariables::AnimationStatesMachineVariables() = default;

SkeletalAnimationVariableId AnimationStatesMachineVariables::registerVariable(const std::string& name,
  float initialValue)
{
  SW_ASSERT(m_variablesNameIdMap.count(name) == 0);

  m_variablesValues.push_back(initialValue);
  m_variablesNameIdMap[name] = static_cast<SkeletalAnimationVariableId>(m_variablesValues.size()) - 1;

  return static_cast<SkeletalAnimationVariableId>(m_variablesValues.size()) - 1;
}

SkeletalAnimationVariableId AnimationStatesMachineVariables::getVariableId(const std::string& name) const
{
  return m_variablesNameIdMap.at(name);
}

void AnimationStatesMachineVariables::setVariableValue(const std::string& name, float value)
{
  m_variablesValues[static_cast<size_t>(getVariableId(name))] = value;
}

float AnimationStatesMachineVariables::getVariableValue(const std::string& name) const
{
  return m_variablesValues[static_cast<size_t>(getVariableId(name))];
}

float AnimationStatesMachineVariables::getVariableValue(SkeletalAnimationVariableId id) const
{
  SW_ASSERT(id < SkeletalAnimationVariableId(m_variablesValues.size()));

  return m_variablesValues[static_cast<size_t>(id)];
}

void AnimationStatesMachineVariables::setVariableValue(SkeletalAnimationVariableId id, float value)
{
  SW_ASSERT(id < SkeletalAnimationVariableId(m_variablesValues.size()));

  m_variablesValues[static_cast<size_t>(id)] = value;
}
