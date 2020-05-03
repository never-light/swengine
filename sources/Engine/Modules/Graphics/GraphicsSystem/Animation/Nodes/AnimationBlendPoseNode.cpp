#include "precompiled.h"

#pragma hdrstop

#include "AnimationBlendPoseNode.h"

AnimationBlendPoseNode::AnimationBlendPoseNode(std::shared_ptr<AnimationPoseNode> firstNode,
  std::shared_ptr<AnimationPoseNode> secondNode,
  SkeletalAnimationVariableId blendParameterVariableId,
  SkeletalAnimationBlendPoseType blendType,
  uint8_t overriddenBone)
  : m_firstNode(firstNode),
    m_secondNode(secondNode),
    m_blendType(blendType),
    m_blendParameterVariableId(blendParameterVariableId),
    m_blendedPose(AnimationPose(m_firstNode->getCurrentPose()))
{
  SW_ASSERT(firstNode->getCurrentPose().getSkeleton() == secondNode->getCurrentPose().getSkeleton());

  uint8_t bonesCount = m_blendedPose.getBonesCount();
  m_overrideMask.resize(bonesCount);

  fillOverrideMask(overriddenBone);
}

AnimationBlendPoseNode::~AnimationBlendPoseNode() = default;

const AnimationPose& AnimationBlendPoseNode::getCurrentPose() const
{
  return m_blendedPose;
}

void AnimationBlendPoseNode::increaseCurrentTime(float delta,
  const AnimationStatesMachineVariables& variablesSet)
{
  ARG_UNUSED(variablesSet);

  m_firstNode->increaseCurrentTime(delta, variablesSet);
  m_secondNode->increaseCurrentTime(delta, variablesSet);

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

  AnimationPoseNodeState firstClipState = m_firstNode->getState();
  AnimationPoseNodeState secondClipState = m_secondNode->getState();

  if (firstClipState == AnimationPoseNodeState::Finished &&
    secondClipState == AnimationPoseNodeState::Finished) {
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
    uint8_t parentId = m_blendedPose.getSkeleton()->getBoneParentId(uint8_t(boneIndex));

    while (parentId != Bone::ROOT_BONE_PARENT_ID) {
      if (m_overrideMask[parentId]) {
        m_overrideMask[boneIndex] = true;
      }

      parentId = m_blendedPose.getSkeleton()->getBoneParentId(parentId);
    }
  }
}

void AnimationBlendPoseNode::linearBlendPoses(const AnimationStatesMachineVariables& variablesSet)
{
  AnimationPose::interpolate(m_firstNode->getCurrentPose(), m_secondNode->getCurrentPose(),
    variablesSet.getVariableValue(m_blendParameterVariableId), m_overrideMask, m_blendedPose);
}

void AnimationBlendPoseNode::overriddenBlendPoses(const AnimationStatesMachineVariables& variablesSet)
{
  ARG_UNUSED(variablesSet);

  const AnimationPose& firstClipPose = m_firstNode->getCurrentPose();
  const AnimationPose& secondClipPose = m_secondNode->getCurrentPose();

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
  AnimationPoseNodeState firstClipState = m_firstNode->getState();
  AnimationPoseNodeState secondClipState = m_secondNode->getState();

  SW_ASSERT(firstClipState == AnimationPoseNodeState::NotStarted ||
    firstClipState == AnimationPoseNodeState::Paused ||
    secondClipState == AnimationPoseNodeState::NotStarted ||
    secondClipState == AnimationPoseNodeState::Paused);

  m_state = AnimationPoseNodeState::Active;

  m_firstNode->startAnimation();
  m_secondNode->startAnimation();
}

void AnimationBlendPoseNode::pauseAnimation()
{
  m_state = AnimationPoseNodeState::Paused;
}

void AnimationBlendPoseNode::resetAnimation()
{
  m_state = AnimationPoseNodeState::NotStarted;
  m_firstNode->resetAnimation();
  m_secondNode->resetAnimation();
}

void AnimationBlendPoseNode::setFinalAction(AnimationPoseNodeFinalAction action)
{
  m_finalAction = action;

  m_firstNode->setFinalAction(action);
  m_secondNode->setFinalAction(action);
}

[[nodiscard]] AnimationPoseNodeFinalAction AnimationBlendPoseNode::getFinalAction() const
{
  return m_finalAction;
}
