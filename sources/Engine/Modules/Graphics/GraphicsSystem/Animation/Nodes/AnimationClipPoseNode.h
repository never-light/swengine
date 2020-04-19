#pragma once

#include <Modules/Graphics/GraphicsSystem/Animation/AnimationPose.h>
#include <Modules/Graphics/GraphicsSystem/Animation/AnimationClipInstance.h>

#include "AnimationPoseNode.h"

class SkeletalAnimationClipPoseNode : public AnimationPoseNode {
 public:
  explicit SkeletalAnimationClipPoseNode(const AnimationClipInstance& clip);
  ~SkeletalAnimationClipPoseNode() override = default;

  [[nodiscard]] const AnimationPose& getCurrentPose() const override;

  void increaseCurrentTime(float delta,
    const AnimationStatesMachineVariables& variablesSet) override;

  void startAnimation() override;
  void pauseAnimation() override;
  void resetAnimation() override;

  [[nodiscard]] AnimationPoseNodeState getState() const override;

  void setFinalAction(AnimationPoseNodeFinalAction action) override;
  [[nodiscard]] AnimationPoseNodeFinalAction getFinalAction() const override;

 private:
  AnimationClipInstance m_clip;
  AnimationPoseNodeState m_state = AnimationPoseNodeState::NotStarted;
  AnimationPoseNodeFinalAction m_finalAction = AnimationPoseNodeFinalAction::Stop;
};
