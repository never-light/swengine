#pragma once

#include "AnimationPoseNode.h"
#include "Modules/Graphics/GraphicsSystem/Animation/AnimationClipInstance.h"

enum class SkeletalAnimationBlendPoseType {
  Linear, Override
};

class AnimationBlendPoseNode : public AnimationPoseNode {
 public:
  AnimationBlendPoseNode(std::shared_ptr<AnimationPoseNode> firstNode,
    std::shared_ptr<AnimationPoseNode> secondNode,
    SkeletalAnimationVariableId blendParameterVariableId,
    SkeletalAnimationBlendPoseType blendType = SkeletalAnimationBlendPoseType::Linear,
    uint8_t overriddenBone = 0);

  ~AnimationBlendPoseNode() override;

  const AnimationPose& getCurrentPose() const override;

  void increaseCurrentTime(float delta,
    const AnimationStatesMachineVariables& variablesSet) override;

  void setBlendParameterVariableId(SkeletalAnimationVariableId variableId);
  SkeletalAnimationVariableId getBlendParameterVariableId() const;

  void startAnimation() override;
  void pauseAnimation() override;
  void resetAnimation() override;

  [[nodiscard]] AnimationPoseNodeState getState() const override;

  void setFinalAction(AnimationPoseNodeFinalAction action) override;
  [[nodiscard]] AnimationPoseNodeFinalAction getFinalAction() const override;

 private:
  void fillOverrideMask(uint8_t overriddenBoneId);

  void linearBlendPoses(const AnimationStatesMachineVariables& variablesSet);
  void overriddenBlendPoses(const AnimationStatesMachineVariables& variablesSet);

 private:
  std::shared_ptr<AnimationPoseNode> m_firstNode;
  std::shared_ptr<AnimationPoseNode> m_secondNode;

  SkeletalAnimationBlendPoseType m_blendType;
  SkeletalAnimationVariableId m_blendParameterVariableId;

  std::vector<uint8_t> m_overrideMask;

  AnimationPose m_blendedPose;
  AnimationPoseNodeState m_state = AnimationPoseNodeState::NotStarted;
  AnimationPoseNodeFinalAction m_finalAction = AnimationPoseNodeFinalAction::Stop;
};
