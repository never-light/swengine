#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationState.h"

#include "Exceptions/NotImplementedException.h"

SkeletalAnimationState::SkeletalAnimationState(const std::string& name,
                                               std::unique_ptr<SkeletalAnimationPoseNode> initialPoseNode)
    : m_name(name),
      m_initialPoseNode(std::move(initialPoseNode)) {

}

const std::string& SkeletalAnimationState::getName() const {
  return m_name;
}

void SkeletalAnimationState::setFinalAction(SkeletalAnimationFinalAction action) {
  m_finalAction = action;
}

SkeletalAnimationFinalAction SkeletalAnimationState::getFinalAction() const {
  return m_finalAction;
}

void SkeletalAnimationState::setFinalTransitionStateId(int16_t stateId) {
  m_finalTransitionStateId = stateId;
}

int16_t SkeletalAnimationState::getFinalTransitionStateId() const {
  return m_finalTransitionStateId;
}

bool SkeletalAnimationState::hasTransition(int16_t nextStateId) const {
  return m_nextTransitions.find(nextStateId) != m_nextTransitions.end();
}

void SkeletalAnimationState::increaseCurrentTime(float delta,
                                                 const SkeletalAnimationStatesMachineVariables& variablesSet) {
  m_initialPoseNode->increaseCurrentTime(delta, variablesSet);
}

const SkeletalAnimationPose& SkeletalAnimationState::getCurrentPose() const {
  return m_initialPoseNode->getCurrentPose();
}

SkeletalAnimationClipPoseNode::SkeletalAnimationClipPoseNode(const SkeletalAnimationClipInstance& clip)
    : m_clip(clip) {

}

const SkeletalAnimationPose& SkeletalAnimationClipPoseNode::getCurrentPose() const {
  return m_clip.getAnimationPose();
}

void SkeletalAnimationClipPoseNode::increaseCurrentTime(float delta,
                                                        const SkeletalAnimationStatesMachineVariables& variablesSet) {
  ARG_UNUSED(variablesSet);

  m_clip.increaseCurrentTime(delta);
}

SkeletalAnimationBlendPoseNode::SkeletalAnimationBlendPoseNode(const SkeletalAnimationClipInstance& firstClip,
                                                               const SkeletalAnimationClipInstance& secondClip,
                                                               SkeletalAnimationVariableId blendParameterVariableId,
                                                               SkeletalAnimationBlendPoseType blendType,
                                                               uint8_t overriddenBone)
    : m_firstClip(firstClip),
      m_secondClip(secondClip),
      m_blendType(blendType),
      m_blendParameterVariableId(blendParameterVariableId),
      m_blendedPose(SkeletalAnimationPose(m_firstClip.getSkeletonPtr())) {
  SW_ASSERT(&firstClip.getSkeleton() == &secondClip.getSkeleton());

  uint8_t bonesCount = firstClip.getSkeleton().getBonesCount();
  m_overrideMask.resize(bonesCount);

  fillOverrideMask(overriddenBone);
}

SkeletalAnimationBlendPoseNode::~SkeletalAnimationBlendPoseNode() {

}

const SkeletalAnimationPose& SkeletalAnimationBlendPoseNode::getCurrentPose() const {
  return m_blendedPose;
}

void SkeletalAnimationBlendPoseNode::increaseCurrentTime(float delta,
                                                         const SkeletalAnimationStatesMachineVariables& variablesSet) {
  ARG_UNUSED(variablesSet);

  m_firstClip.increaseCurrentTime(delta);
  m_secondClip.increaseCurrentTime(delta);

  switch (m_blendType) {
    case SkeletalAnimationBlendPoseType::Linear:linearBlendPoses(variablesSet);
      break;

    case SkeletalAnimationBlendPoseType::Override:overriddenBlendPoses(variablesSet);
      break;

    default:break;
  }
}

void SkeletalAnimationBlendPoseNode::setBlendParameterVariableId(SkeletalAnimationVariableId variableId) {
  m_blendParameterVariableId = variableId;
}

SkeletalAnimationVariableId SkeletalAnimationBlendPoseNode::getBlendParameterVariableId() const {
  return m_blendParameterVariableId;
}

void SkeletalAnimationBlendPoseNode::fillOverrideMask(uint8_t overriddenBoneId) {
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

void SkeletalAnimationBlendPoseNode::linearBlendPoses(const SkeletalAnimationStatesMachineVariables& variablesSet) {
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

void SkeletalAnimationBlendPoseNode::overriddenBlendPoses(const SkeletalAnimationStatesMachineVariables& variablesSet) {
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
