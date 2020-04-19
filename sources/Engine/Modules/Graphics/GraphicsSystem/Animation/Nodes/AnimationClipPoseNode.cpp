#include "precompiled.h"

#pragma hdrstop

#include "AnimationClipPoseNode.h"

SkeletalAnimationClipPoseNode::SkeletalAnimationClipPoseNode(const AnimationClipInstance& clip)
  : m_clip(clip)
{

}

const AnimationPose& SkeletalAnimationClipPoseNode::getCurrentPose() const
{
  return m_clip.getAnimationPose();
}

void SkeletalAnimationClipPoseNode::increaseCurrentTime(float delta,
  const AnimationStatesMachineVariables& variablesSet)
{
  ARG_UNUSED(variablesSet);

  if (m_state != AnimationPoseNodeState::Active) {
    return;
  }

  AnimationClipTimeIncrementResult incrementResult = m_clip.increaseCurrentTime(delta);

  if (incrementResult == AnimationClipTimeIncrementResult::Finished) {
    m_state = AnimationPoseNodeState::Finished;
  }
}

void SkeletalAnimationClipPoseNode::startAnimation()
{
  AnimationClipState clipState = m_clip.getCurrentState();

  SW_ASSERT(clipState == AnimationClipState::NotStarted ||
    clipState == AnimationClipState::Paused);

  m_state = AnimationPoseNodeState::Active;
  m_clip.start();
}

void SkeletalAnimationClipPoseNode::pauseAnimation()
{
  m_state = AnimationPoseNodeState::Paused;
}

AnimationPoseNodeState SkeletalAnimationClipPoseNode::getState() const
{
  return m_state;
}

void SkeletalAnimationClipPoseNode::resetAnimation()
{
  m_state = AnimationPoseNodeState::NotStarted;
  m_clip.resetClip();
}

void SkeletalAnimationClipPoseNode::setFinalAction(AnimationPoseNodeFinalAction action)
{
  m_finalAction = action;

  switch (m_finalAction) {
    case AnimationPoseNodeFinalAction::Stop:
      m_clip.setEndBehaviour(AnimationClipEndBehaviour::Stop);
      break;

    case AnimationPoseNodeFinalAction::Repeat:
      m_clip.setEndBehaviour(AnimationClipEndBehaviour::Repeat);
      break;

    default:
      SW_ASSERT(false);
  }
}

AnimationPoseNodeFinalAction SkeletalAnimationClipPoseNode::getFinalAction() const
{
  return m_finalAction;
}