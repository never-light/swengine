#include "precompiled.h"

#pragma hdrstop

#include "AnimationBlendPoseNode.h"

AnimationBlendPoseNode::AnimationBlendPoseNode(const AnimationClipInstance& firstClip,
  const AnimationClipInstance& secondClip,
  SkeletalAnimationVariableId blendParameterVariableId,
  SkeletalAnimationBlendPoseType blendType,
  uint8_t overriddenBone)
  : m_firstClip(firstClip),
    m_secondClip(secondClip),
    m_blendType(blendType),
    m_blendParameterVariableId(blendParameterVariableId),
    m_blendedPose(AnimationPose(m_firstClip.getSkeletonPtr()))
{
  SW_ASSERT(&firstClip.getSkeleton() == &secondClip.getSkeleton());

  uint8_t bonesCount = firstClip.getSkeleton().getBonesCount();
  m_overrideMask.resize(bonesCount);

  fillOverrideMask(overriddenBone);
}

AnimationBlendPoseNode::~AnimationBlendPoseNode()
{

}

const AnimationPose& AnimationBlendPoseNode::getCurrentPose() const
{
  return m_blendedPose;
}

void AnimationBlendPoseNode::increaseCurrentTime(float delta,
  const AnimationStatesMachineVariables& variablesSet)
{
  ARG_UNUSED(variablesSet);

  RETURN_VALUE_UNUSED(m_firstClip.increaseCurrentTime(delta));
  RETURN_VALUE_UNUSED(m_secondClip.increaseCurrentTime(delta));

  switch (m_blendType) {
    case SkeletalAnimationBlendPoseType::Linear:
      linearBlendPoses(variablesSet);
      break;

    case SkeletalAnimationBlendPoseType::Override:
      overriddenBlendPoses(variablesSet);
      break;

    default:
      break;
  }

  AnimationClipState firstClipState = m_firstClip.getCurrentState();
  AnimationClipState secondClipState = m_secondClip.getCurrentState();

  if (firstClipState == AnimationClipState::Finished &&
    secondClipState == AnimationClipState::Finished) {
    m_state = AnimationPoseNodeState::Finished;
  }
}

void AnimationBlendPoseNode::setBlendParameterVariableId(SkeletalAnimationVariableId variableId)
{
  m_blendParameterVariableId = variableId;
}

SkeletalAnimationVariableId AnimationBlendPoseNode::getBlendParameterVariableId() const
{
  return m_blendParameterVariableId;
}

void AnimationBlendPoseNode::fillOverrideMask(uint8_t overriddenBoneId)
{
  for (size_t i = 0; i < m_overrideMask.size(); i++) {
    m_overrideMask[i] = false;
  }

  m_overrideMask[overriddenBoneId] = true;

  for (size_t boneIndex = 1; boneIndex < m_overrideMask.size(); boneIndex++) {
    uint8_t parentId = m_firstClip.getSkeleton().getBoneParentId(uint8_t(boneIndex));

    while (parentId != Bone::ROOT_BONE_PARENT_ID) {
      if (m_overrideMask[parentId]) {
        m_overrideMask[boneIndex] = true;
      }

      parentId = m_firstClip.getSkeleton().getBoneParentId(parentId);
    }
  }
}

void AnimationBlendPoseNode::linearBlendPoses(const AnimationStatesMachineVariables& variablesSet)
{
  AnimationPose::interpolate(m_firstClip.getAnimationPose(), m_secondClip.getAnimationPose(),
    variablesSet.getVariableValue(m_blendParameterVariableId), m_overrideMask, m_blendedPose);
}

void AnimationBlendPoseNode::overriddenBlendPoses(const AnimationStatesMachineVariables& variablesSet)
{
  ARG_UNUSED(variablesSet);

  const AnimationPose& firstClipPose = m_firstClip.getAnimationPose();
  const AnimationPose& secondClipPose = m_secondClip.getAnimationPose();

  for (uint8_t boneIndex = 0; boneIndex < m_overrideMask.size(); boneIndex++) {
    if (m_overrideMask[boneIndex]) {
      m_blendedPose.setBoneLocalPose(boneIndex, secondClipPose.getBoneLocalPose(boneIndex));
    }
    else {
      m_blendedPose.setBoneLocalPose(boneIndex, firstClipPose.getBoneLocalPose(boneIndex));
    }
  }
}

AnimationPoseNodeState AnimationBlendPoseNode::getState() const
{
  return m_state;
}

void AnimationBlendPoseNode::startAnimation()
{
  AnimationClipState firstClipState = m_firstClip.getCurrentState();
  AnimationClipState secondClipState = m_secondClip.getCurrentState();

  SW_ASSERT(firstClipState == AnimationClipState::NotStarted ||
    firstClipState == AnimationClipState::Paused ||
    secondClipState == AnimationClipState::NotStarted ||
    secondClipState == AnimationClipState::Paused);

  m_state = AnimationPoseNodeState::Active;

  m_firstClip.start();
  m_secondClip.start();
}

void AnimationBlendPoseNode::pauseAnimation()
{
  m_state = AnimationPoseNodeState::Paused;
}

void AnimationBlendPoseNode::resetAnimation()
{
  m_state = AnimationPoseNodeState::NotStarted;
  m_firstClip.resetClip();
  m_secondClip.resetClip();
}

void AnimationBlendPoseNode::setFinalAction(AnimationPoseNodeFinalAction action)
{
  m_finalAction = action;

  switch (m_finalAction) {
    case AnimationPoseNodeFinalAction::Stop:
      m_firstClip.setEndBehaviour(AnimationClipEndBehaviour::Stop);
      m_secondClip.setEndBehaviour(AnimationClipEndBehaviour::Stop);
      break;

    case AnimationPoseNodeFinalAction::Repeat:
      m_firstClip.setEndBehaviour(AnimationClipEndBehaviour::Repeat);
      m_secondClip.setEndBehaviour(AnimationClipEndBehaviour::Repeat);
      break;

    default:
      SW_ASSERT(false);
  }
}

[[nodiscard]] AnimationPoseNodeFinalAction AnimationBlendPoseNode::getFinalAction() const
{
  return m_finalAction;
}
