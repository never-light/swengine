#include "precompiled.h"

#pragma hdrstop

#include "AnimationState.h"

#include "Exceptions/NotImplementedException.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Nodes/AnimationPoseNode.h"
#include "Bone.h"
#include "AnimationClipInstance.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Nodes/AnimationClipPoseNode.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Nodes/AnimationBlendPoseNode.h"
#include "SkeletalAnimationPose.h"

AnimationState::AnimationState(const std::string& name,
  std::unique_ptr<AnimationPoseNode> initialPoseNode)
  : m_name(name),
    m_initialPoseNode(std::move(initialPoseNode))
{

}

const std::string& AnimationState::getName() const
{
  return m_name;
}

void AnimationState::setFinalAction(SkeletalAnimationFinalAction action)
{
  m_finalAction = action;
}

SkeletalAnimationFinalAction AnimationState::getFinalAction() const
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

bool AnimationState::hasTransition(int16_t nextStateId) const
{
  return m_nextTransitions.find(nextStateId) != m_nextTransitions.end();
}

void AnimationState::increaseCurrentTime(float delta,
  const AnimationStatesMachineVariables& variablesSet)
{
  m_initialPoseNode->increaseCurrentTime(delta, variablesSet);
}

const SkeletalAnimationPose& AnimationState::getCurrentPose() const
{
  return m_initialPoseNode->getCurrentPose();
}

