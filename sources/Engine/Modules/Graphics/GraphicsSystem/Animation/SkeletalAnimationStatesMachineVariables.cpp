#include "precompiled.h"
#pragma hdrstop

#include "SkeletalAnimationStatesMachineVariables.h"

SkeletalAnimationStatesMachineVariables::SkeletalAnimationStatesMachineVariables()
{

}

size_t SkeletalAnimationStatesMachineVariables::registerVaraible(const std::string& name, float initialValue)
{
    SW_ASSERT(m_variablesNameIdMap.count(name) == 0);

    m_variablesValues.push_back(initialValue);
    m_variablesNameIdMap[name] = m_variablesValues.size() - 1;

    return m_variablesValues.size() - 1;
}

size_t SkeletalAnimationStatesMachineVariables::getVariableId(const std::string& name) const
{
    return m_variablesNameIdMap.at(name);
}

void SkeletalAnimationStatesMachineVariables::setVariableValue(const std::string& name, float value)
{
    m_variablesValues[getVariableId(name)] = value;
}

float SkeletalAnimationStatesMachineVariables::getVariableValue(const std::string& name) const
{
    return m_variablesValues[getVariableId(name)];
}

float SkeletalAnimationStatesMachineVariables::getVariableValue(size_t id) const
{
    SW_ASSERT(id < m_variablesValues.size());

    return m_variablesValues[id];
}

void SkeletalAnimationStatesMachineVariables::setVariableValue(size_t id, float value)
{
    SW_ASSERT(id < m_variablesValues.size());

    m_variablesValues[id] = value;
}
