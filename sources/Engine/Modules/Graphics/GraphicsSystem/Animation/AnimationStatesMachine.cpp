#include "precompiled.h"

#pragma hdrstop

#include <Exceptions/NotImplementedException.h>

#include "AnimationStatesMachine.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Nodes/AnimationPoseNode.h"
#include "AnimationClipInstance.h"
#include "SkeletalAnimationPose.h"

AnimationStatesMachine::AnimationStatesMachine(std::shared_ptr<Skeleton> skeleton)
  : m_skeleton(skeleton)
{

}

int16_t AnimationStatesMachine::getStateIdByName(const std::string& name) const
{
  return m_statesNameToIdMap.at(name);
}

AnimationState& AnimationStatesMachine::addState(const std::string& name,
  std::unique_ptr<AnimationPoseNode> initialPoseNode)
{
  int16_t newStateId = static_cast<int16_t>(m_states.size());

  m_states.push_back(AnimationState(name, std::move(initialPoseNode)));
  m_statesNameToIdMap[name] = newStateId;

  m_states.rbegin()->m_id = newStateId;

  return *m_states.rbegin();
}

void AnimationStatesMachine::addTransition(int16_t sourceStateId, int16_t targetStateId)
{
  SW_ASSERT(sourceStateId >= 0 && sourceStateId < int16_t(m_states.size()) &&
    !m_states[size_t(sourceStateId)].hasTransition(targetStateId));

  m_states[size_t(sourceStateId)].m_nextTransitions.insert(targetStateId);
}

void AnimationStatesMachine::switchToNextState(int16_t stateId)
{
  SW_ASSERT(m_activeStateId > 0 && m_states[size_t(m_activeStateId)].hasTransition(stateId));

  m_activeStateId = stateId;
}

void AnimationStatesMachine::setActiveState(int16_t stateId)
{
  SW_ASSERT(stateId >= 0 && stateId < int16_t(m_states.size()));

  m_activeStateId = stateId;
}

const AnimationState& AnimationStatesMachine::getActiveState() const
{
  SW_ASSERT(m_activeStateId != -1);

  return m_states[size_t(m_activeStateId)];
}

AnimationState& AnimationStatesMachine::getActiveState()
{
  SW_ASSERT(m_activeStateId != -1);

  return m_states[size_t(m_activeStateId)];
}

const SkeletalAnimationPose& AnimationStatesMachine::getCurrentPose() const
{
  return getActiveState().getCurrentPose();
}

const AnimationMatrixPalette& AnimationStatesMachine::getCurrentMatrixPalette() const
{
  return getCurrentPose().getMatrixPalette();
}

const AnimationStatesMachineVariables& AnimationStatesMachine::getVariablesSet() const
{
  return m_variablesSet;
}

AnimationStatesMachineVariables& AnimationStatesMachine::getVariablesSet()
{
  return m_variablesSet;
}

void AnimationStatesMachine::increaseCurrentTime(float delta)
{
  getActiveState().increaseCurrentTime(delta, m_variablesSet);
}
