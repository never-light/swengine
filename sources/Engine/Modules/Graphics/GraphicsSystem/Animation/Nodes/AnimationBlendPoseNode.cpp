#include "precompiled.h"

#pragma hdrstop

#include "AnimationBlendPoseNode.h"

AnimationBlendPoseNode::AnimationBlendPoseNode(const SkeletalAnimationClipInstance& firstClip,
  const SkeletalAnimationClipInstance& secondClip,
  SkeletalAnimationVariableId blendParameterVariableId,
  SkeletalAnimationBlendPoseType blendType,
  uint8_t overriddenBone)
  : m_firstClip(firstClip),
    m_secondClip(secondClip),
    m_blendType(blendType),
    m_blendParameterVariableId(blendParameterVariableId),
    m_blendedPose(SkeletalAnimationPose(m_firstClip.getSkeletonPtr()))
{
  SW_ASSERT(&firstClip.getSkeleton() == &secondClip.getSkeleton());

  uint8_t bonesCount = firstClip.getSkeleton().getBonesCount();
  m_overrideMask.resize(bonesCount);

  fillOverrideMask(overriddenBone);
}

AnimationBlendPoseNode::~AnimationBlendPoseNode()
{

}

const SkeletalAnimationPose& AnimationBlendPoseNode::getCurrentPose() const
{
  return m_blendedPose;
}

void AnimationBlendPoseNode::increaseCurrentTime(float delta,
  const AnimationStatesMachineVariables& variablesSet)
{
  ARG_UNUSED(variablesSet);

  m_firstClip.increaseCurrentTime(delta);
  m_secondClip.increaseCurrentTime(delta);

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

    while (parentId != 0) {
      if (parentId == overriddenBoneId) {
        m_overrideMask[boneIndex] = true;
      }

      parentId = m_firstClip.getSkeleton().getBoneParentId(parentId);
    }
  }
}

void AnimationBlendPoseNode::linearBlendPoses(const AnimationStatesMachineVariables& variablesSet)
{
  const SkeletalAnimationPose& firstClipPose = m_firstClip.getAnimationPose();
  const SkeletalAnimationPose& secondClipPose = m_secondClip.getAnimationPose();

  for (uint8_t boneIndex = 0; boneIndex < m_overrideMask.size(); boneIndex++) {
    if (m_overrideMask[boneIndex]) {
      float blendParameterValue = variablesSet.getVariableValue(m_blendParameterVariableId);
      BonePose interpolatedBonePose = BonePose::interpolate(firstClipPose.getBoneLocalPose(boneIndex),
        secondClipPose.getBoneLocalPose(boneIndex),
        blendParameterValue);

      m_blendedPose.setBoneLocalPose(boneIndex, interpolatedBonePose);
    }
    else {
      m_blendedPose.setBoneLocalPose(boneIndex, firstClipPose.getBoneLocalPose(boneIndex));
    }
  }
}

void AnimationBlendPoseNode::overriddenBlendPoses(const AnimationStatesMachineVariables& variablesSet)
{
  ARG_UNUSED(variablesSet);

  const SkeletalAnimationPose& firstClipPose = m_firstClip.getAnimationPose();
  const SkeletalAnimationPose& secondClipPose = m_secondClip.getAnimationPose();

  for (uint8_t boneIndex = 0; boneIndex < m_overrideMask.size(); boneIndex++) {
    if (m_overrideMask[boneIndex]) {
      m_blendedPose.setBoneLocalPose(boneIndex, secondClipPose.getBoneLocalPose(boneIndex));
    }
    else {
      m_blendedPose.setBoneLocalPose(boneIndex, firstClipPose.getBoneLocalPose(boneIndex));
    }
  }
}