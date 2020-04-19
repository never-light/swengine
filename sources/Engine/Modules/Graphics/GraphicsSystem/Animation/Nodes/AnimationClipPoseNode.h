#pragma once

#include <Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationPose.h>
#include <Modules/Graphics/GraphicsSystem/Animation/AnimationClipInstance.h>

#include "AnimationPoseNode.h"

class SkeletalAnimationClipPoseNode : public AnimationPoseNode {
 public:
  explicit SkeletalAnimationClipPoseNode(const SkeletalAnimationClipInstance& clip);
  ~SkeletalAnimationClipPoseNode() override = default;

  [[nodiscard]] const SkeletalAnimationPose& getCurrentPose() const override;

  void increaseCurrentTime(float delta,
    const AnimationStatesMachineVariables& variablesSet) override;

 private:
  SkeletalAnimationClipInstance m_clip;
};
