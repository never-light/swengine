#include "precompiled.h"

#pragma hdrstop

#include "AnimationClipPoseNode.h"

SkeletalAnimationClipPoseNode::SkeletalAnimationClipPoseNode(const SkeletalAnimationClipInstance& clip)
  : m_clip(clip)
{

}

const SkeletalAnimationPose& SkeletalAnimationClipPoseNode::getCurrentPose() const
{
  return m_clip.getAnimationPose();
}

void SkeletalAnimationClipPoseNode::increaseCurrentTime(float delta,
  const AnimationStatesMachineVariables& variablesSet)
{
  ARG_UNUSED(variablesSet);

  m_clip.increaseCurrentTime(delta);
}