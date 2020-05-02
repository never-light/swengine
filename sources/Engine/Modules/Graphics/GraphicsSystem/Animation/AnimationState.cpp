#include "precompiled.h"

#pragma hdrstop

#include "AnimationState.h"

#include "Exceptions/NotImplementedException.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Nodes/AnimationPoseNode.h"
#include "Bone.h"
#include "AnimationClipInstance.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Nodes/AnimationClipPoseNode.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Nodes/AnimationBlendPoseNode.h"
#include "AnimationPose.h"

AnimationState::AnimationState(const std::string& name,
  std::unique_ptr<AnimationPoseNode> initialPoseNode)
  : m_name(name),
    m_initialPoseNode(std::move(initialPoseNode))
{
  SW_ASSERT(m_initialPoseNode != nullptr);
}

const std::string& AnimationState::getName() const
{
  return m_name;
}

void AnimationState::setFinalAction(AnimationFinalAction action)
{
  m_finalAction = action;

  switch (m_finalAction) {
    case AnimationFinalAction::Repeat:
      m_initialPoseNode->setFinalAction(AnimationPoseNodeFinalAction::Repeat);
      break;

    case AnimationFinalAction::Stop:
    case AnimationFinalAction::SwitchState:
      m_initialPoseNode->setFinalAction(AnimationPoseNodeFinalAction::Stop);
      break;

    default:
      SW_ASSERT(false);
      break;
  }
}

AnimationFinalAction AnimationState::getFinalAction() const
{
  return m_finalAction;
}

void AnimationState::setFinalTransitionStateId(int16_t stateId)
{
  m_finalTransitionStateId = stateId;
}

int16_t AnimationState::getFinalTransitionStateId() const
{
  return m_finalTransitionStateId;
}

void AnimationState::increaseCurrentTime(float delta,
  const AnimationStatesMachineVariables& variablesSet)
{
  m_initialPoseNode->increaseCurrentTime(delta, variablesSet);

  if (m_initialPoseNode->getState() == AnimationPoseNodeState::Active) {
    m_currentTime += delta;
  }
}

const AnimationPose& AnimationState::getCurrentPose() const
{
  return m_initialPoseNode->getCurrentPose();
}

void AnimationState::activate()
{
  m_initialPoseNode->startAnimation();
  m_currentTime = 0.0f;
}

void AnimationState::deactivate()
{
  m_initialPoseNode->resetAnimation();
}

const AnimationPoseNode& AnimationState::getInitialPoseNode() const
{
  return *m_initialPoseNode;
}

float AnimationState::getCurrentTime() const
{
  return m_currentTime;
}