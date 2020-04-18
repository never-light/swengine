#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationStatesMachineVariables.h"

SkeletalAnimationStatesMachineVariables::SkeletalAnimationStatesMachineVariables()
{

}

SkeletalAnimationVariableId SkeletalAnimationStatesMachineVariables::registerVariable(const std::string& name,
  float initialValue)
{
  SW_ASSERT(m_variablesNameIdMap.count(name) == 0);

  m_variablesValues.push_back(initialValue);
  m_variablesNameIdMap[name] = static_cast<SkeletalAnimationVariableId>(m_variablesValues.size()) - 1;

  return static_cast<SkeletalAnimationVariableId>(m_variablesValues.size()) - 1;
}

SkeletalAnimationVariableId SkeletalAnimationStatesMachineVariables::getVariableId(const std::string& name) const
{
  return m_variablesNameIdMap.at(name);
}

void SkeletalAnimationStatesMachineVariables::setVariableValue(const std::string& name, float value)
{
  m_variablesValues[static_cast<size_t>(getVariableId(name))] = value;
}

float SkeletalAnimationStatesMachineVariables::getVariableValue(const std::string& name) const
{
  return m_variablesValues[static_cast<size_t>(getVariableId(name))];
}

float SkeletalAnimationStatesMachineVariables::getVariableValue(SkeletalAnimationVariableId id) const
{
  SW_ASSERT(id < SkeletalAnimationVariableId(m_variablesValues.size()));

  return m_variablesValues[static_cast<size_t>(id)];
}

void SkeletalAnimationStatesMachineVariables::setVariableValue(SkeletalAnimationVariableId id, float value)
{
  SW_ASSERT(id < SkeletalAnimationVariableId(m_variablesValues.size()));

  m_variablesValues[static_cast<size_t>(id)] = value;
}
