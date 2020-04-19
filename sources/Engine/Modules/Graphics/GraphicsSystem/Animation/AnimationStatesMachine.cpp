#include "precompiled.h"

#pragma hdrstop

#include <Exceptions/NotImplementedException.h>

#include "AnimationStatesMachine.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Nodes/AnimationPoseNode.h"
#include "AnimationClipInstance.h"
#include "AnimationPose.h"

AnimationStatesMachine::AnimationStatesMachine(std::shared_ptr<Skeleton> skeleton)
  : m_skeleton(skeleton)
{

}

int16_t AnimationStatesMachine::getStateIdByName(const std::string& name) const
{
  return m_statesNameToIdMap.at(name);
}

AnimationState& AnimationStatesMachine::getState(int16_t stateId)
{
  SW_ASSERT(stateId >= 0 && stateId < int16_t(m_states.size()));

  return m_states[stateId];
}

const AnimationState& AnimationStatesMachine::getState(int16_t stateId) const
{
  SW_ASSERT(stateId >= 0 && stateId < int16_t(m_states.size()));

  return m_states[stateId];
}

void AnimationStatesMachine::addState(const std::string& name,
  std::unique_ptr<AnimationPoseNode> initialPoseNode)
{
  auto newStateId = static_cast<int16_t>(m_states.size());

  m_states.emplace_back(name, std::move(initialPoseNode));
  m_statesNameToIdMap[name] = newStateId;

  m_states.rbegin()->m_id = newStateId;

  for (auto& transitionsTableEntry : m_transitionsTable) {
    transitionsTableEntry.push_back(false);
  }

  m_transitionsTable.emplace_back(m_states.size(), 0);
}

void AnimationStatesMachine::addTransition(int16_t sourceStateId, int16_t targetStateId)
{
  SW_ASSERT(sourceStateId >= 0 && sourceStateId < int16_t(m_states.size()) &&
    targetStateId >= 0 && targetStateId <= int16_t(m_states.size()) &&
    !m_transitionsTable[sourceStateId][targetStateId]);

  m_transitionsTable[sourceStateId][targetStateId] = true;
}

void AnimationStatesMachine::switchToNextState(int16_t stateId)
{
  SW_ASSERT(m_activeStateId >= 0 && m_transitionsTable[m_activeStateId][stateId]);

  getActiveState().deactivate();

  m_activeStateId = stateId;
  getActiveState().activate();
}

void AnimationStatesMachine::setActiveState(int16_t stateId)
{
  SW_ASSERT(stateId >= 0 && stateId < int16_t(m_states.size()));

  if (m_activeStateId != INVALID_STATE_ID) {
    getActiveState().deactivate();
  }

  m_activeStateId = stateId;
  getActiveState().activate();
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

const AnimationPose& AnimationStatesMachine::getCurrentPose() const
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
  AnimationState& activeState = getActiveState();

  activeState.increaseCurrentTime(delta, m_variablesSet);

  if (activeState.getInitialPoseNode().getState() == AnimationPoseNodeState::Finished) {
    if (activeState.getFinalAction() == AnimationFinalAction::SwitchState) {
      switchToNextState(activeState.getFinalTransitionStateId());
    }
  }
}

int16_t AnimationStatesMachine::getActiveStateId() const
{
  return m_activeStateId;
}

bool AnimationStatesMachine::isActive() const
{
  return m_activeStateId != INVALID_STATE_ID;
}

