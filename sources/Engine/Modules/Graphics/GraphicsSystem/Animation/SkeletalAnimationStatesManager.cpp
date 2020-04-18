#include "precompiled.h"

#pragma hdrstop

#include <Exceptions/NotImplementedException.h>

#include "SkeletalAnimationStatesManager.h"

SkeletalAnimationStatesMachine::SkeletalAnimationStatesMachine(std::shared_ptr<Skeleton> skeleton)
  : m_skeleton(skeleton)
{

}

int16_t SkeletalAnimationStatesMachine::getStateIdByName(const std::string& name) const
{
  return m_statesNameToIdMap.at(name);
}

SkeletalAnimationState& SkeletalAnimationStatesMachine::addState(const std::string& name,
  std::unique_ptr<SkeletalAnimationPoseNode> initialPoseNode)
{
  int16_t newStateId = static_cast<int16_t>(m_states.size());

  m_states.push_back(SkeletalAnimationState(name, std::move(initialPoseNode)));
  m_statesNameToIdMap[name] = newStateId;

  m_states.rbegin()->m_id = newStateId;

  return *m_states.rbegin();
}

void SkeletalAnimationStatesMachine::addTransition(int16_t sourceStateId, int16_t targetStateId)
{
  SW_ASSERT(sourceStateId >= 0 && sourceStateId < int16_t(m_states.size()) &&
    m_states[size_t(sourceStateId)].hasTransition(targetStateId));

  m_states[size_t(sourceStateId)].m_nextTransitions.insert(targetStateId);
}

void SkeletalAnimationStatesMachine::switchToNextState(int16_t stateId)
{
  SW_ASSERT(m_activeStateId > 0 && m_states[size_t(m_activeStateId)].hasTransition(stateId));

  m_activeStateId = stateId;
}

void SkeletalAnimationStatesMachine::setActiveState(int16_t stateId)
{
  SW_ASSERT(stateId >= 0 && stateId < int16_t(m_states.size()));

  m_activeStateId = stateId;
}

const SkeletalAnimationState& SkeletalAnimationStatesMachine::getActiveState() const
{
  SW_ASSERT(m_activeStateId != -1);

  return m_states[size_t(m_activeStateId)];
}

SkeletalAnimationState& SkeletalAnimationStatesMachine::getActiveState()
{
  SW_ASSERT(m_activeStateId != -1);

  return m_states[size_t(m_activeStateId)];
}

const SkeletalAnimationPose& SkeletalAnimationStatesMachine::getCurrentPose() const
{
  return getActiveState().getCurrentPose();
}

const SkeletalAnimationMatrixPalette& SkeletalAnimationStatesMachine::getCurrentMatrixPalette() const
{
  return getCurrentPose().getMatrixPalette();
}

const SkeletalAnimationStatesMachineVariables& SkeletalAnimationStatesMachine::getVariablesSet() const
{
  return m_variablesSet;
}

SkeletalAnimationStatesMachineVariables& SkeletalAnimationStatesMachine::getVariablesSet()
{
  return m_variablesSet;
}

void SkeletalAnimationStatesMachine::increaseCurrentTime(float delta)
{
  getActiveState().increaseCurrentTime(delta, m_variablesSet);
}
