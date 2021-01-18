#include "precompiled.h"

#pragma hdrstop

#include <Exceptions/NotImplementedException.h>

#include "AnimationStatesMachine.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Nodes/AnimationPoseNode.h"
#include "AnimationClipInstance.h"
#include "AnimationPose.h"

AnimationStatesMachine::AnimationStatesMachine(ResourceHandle<Skeleton> skeleton)
  : m_skeleton(skeleton),
    m_fadingPose(skeleton),
    m_smoothedPose(skeleton)
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
  std::shared_ptr<AnimationPoseNode> initialPoseNode)
{
  auto newStateId = static_cast<int16_t>(m_states.size());

  m_states.emplace_back(name, initialPoseNode);
  m_statesNameToIdMap[name] = newStateId;

  m_states.rbegin()->m_id = newStateId;

  for (auto& transitionsTableEntry : m_transitionsTable) {
    transitionsTableEntry.push_back(AnimationTransition(AnimationStatesTransitionType::Straight,
      0.0f, false));
  }

  m_transitionsTable.emplace_back(m_states.size(), AnimationTransition(AnimationStatesTransitionType::Straight,
    0.0f, false));

  m_activeTransition = nullptr;
}

void AnimationStatesMachine::setTransition(int16_t sourceStateId, int16_t targetStateId, AnimationTransition transition)
{
  SW_ASSERT(sourceStateId >= 0 && sourceStateId < int16_t(m_states.size()) &&
    targetStateId >= 0 && targetStateId <= int16_t(m_states.size()));

  m_transitionsTable[sourceStateId][targetStateId] = transition;
}

void AnimationStatesMachine::switchToNextState(int16_t stateId)
{
  SW_ASSERT(m_activeStateId >= 0 && m_transitionsTable[m_activeStateId][stateId].isActive());

  AnimationTransition& transition = m_transitionsTable[m_activeStateId][stateId];

  if (transition.getType() == AnimationStatesTransitionType::SmoothLinear) {
    m_fadingPose = getCurrentPose();
    m_activeTransition = &transition;
  }

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
  if (isTransitionActive()) {
    return m_smoothedPose;
  }
  else {
    return getActiveState().getCurrentPose();
  }
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

  if (isTransitionActive()) {
    if (activeState.getCurrentTime() > m_activeTransition->getDuration()) {
      finishActiveTransition();
    }
    else {
      float transitionBlendFactor = glm::clamp(getActiveState().getCurrentTime() / m_activeTransition->getDuration(),
        0.0f, 1.0f);

      AnimationPose::interpolate(m_fadingPose,
        getActiveState().getCurrentPose(), transitionBlendFactor, m_smoothedPose);
    }
  }

  // Check conditional transitions and switch to next state if any condition is fulfilled
  bool isTransitionActivated = false;

  for (size_t targetIndex = 0; targetIndex < m_transitionsTable[m_activeStateId].size(); targetIndex++) {
    auto& transition = m_transitionsTable[m_activeStateId][targetIndex];

    if (transition.isActive()) {
      std::shared_ptr<ConditionalNode> conditionalNode = transition.getCondition();

      if (conditionalNode != nullptr && std::get<bool>(conditionalNode->evaluate(m_variablesSet))) {
        switchToNextState(static_cast<int16_t>(targetIndex));
        isTransitionActivated = true;
      }
    }
  }

  // Check whether animation state is finished and switch to next state if it is needed
  if (!isTransitionActivated && activeState.getInitialPoseNode().getState() == AnimationPoseNodeState::Finished) {
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

bool AnimationStatesMachine::hasActiveTransition(int16_t sourceStateId, int16_t targetStateId) const
{
  SW_ASSERT(sourceStateId >= 0 && sourceStateId < int16_t(m_states.size()) &&
    targetStateId >= 0 && targetStateId <= int16_t(m_states.size()));

  return m_transitionsTable[sourceStateId][targetStateId].isActive();
}

void AnimationStatesMachine::finishActiveTransition()
{
  m_activeTransition = nullptr;
}

bool AnimationStatesMachine::isTransitionActive() const
{
  return m_activeTransition != nullptr;
}

AnimationState& AnimationStatesMachine::getState(const std::string& name)
{
  return getState(getStateIdByName(name));
}

const AnimationState& AnimationStatesMachine::getState(const std::string& name) const
{
  return getState(getStateIdByName(name));
}

void AnimationStatesMachine::setActiveState(const std::string& stateName)
{
  setActiveState(getStateIdByName(stateName));
}

ResourceHandle<Skeleton> AnimationStatesMachine::getSkeleton() const
{
  return m_skeleton;
}
