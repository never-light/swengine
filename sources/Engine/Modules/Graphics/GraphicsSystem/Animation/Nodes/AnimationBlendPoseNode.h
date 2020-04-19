#pragma once

#include "AnimationPoseNode.h"
#include "Modules/Graphics/GraphicsSystem/Animation/AnimationClipInstance.h"

enum class SkeletalAnimationBlendPoseType {
  Linear, Override
};

class AnimationBlendPoseNode : public AnimationPoseNode {
 public:
  AnimationBlendPoseNode(const SkeletalAnimationClipInstance& firstClip,
    const SkeletalAnimationClipInstance& secondClip,
    SkeletalAnimationVariableId blendParameterVariableId,
    SkeletalAnimationBlendPoseType blendType = SkeletalAnimationBlendPoseType::Linear,
    uint8_t overriddenBone = 0);

  ~AnimationBlendPoseNode() override;

  const SkeletalAnimationPose& getCurrentPose() const override;

  void increaseCurrentTime(float delta,
    const AnimationStatesMachineVariables& variablesSet) override;

  void setBlendParameterVariableId(SkeletalAnimationVariableId variableId);
  SkeletalAnimationVariableId getBlendParameterVariableId() const;

 private:
  void fillOverrideMask(uint8_t overriddenBoneId);

  void linearBlendPoses(const AnimationStatesMachineVariables& variablesSet);
  void overriddenBlendPoses(const AnimationStatesMachineVariables& variablesSet);

 private:
  SkeletalAnimationClipInstance m_firstClip;
  SkeletalAnimationClipInstance m_secondClip;

  SkeletalAnimationBlendPoseType m_blendType;
  SkeletalAnimationVariableId m_blendParameterVariableId;

  std::vector<uint8_t> m_overrideMask;

  SkeletalAnimationPose m_blendedPose;
};
