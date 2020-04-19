#pragma once

#include <Modules/Graphics/GraphicsSystem/Animation/AnimationClip.h>
#include <Modules/Graphics/GraphicsSystem/Animation/AnimationStatesMachineVariables.h>
#include <Modules/Graphics/GraphicsSystem/Animation/AnimationClipInstance.h>
#include <Modules/Graphics/GraphicsSystem//Animation/SkeletalAnimationPose.h>

class AnimationPoseNode {
 public:
  AnimationPoseNode() = default;
  virtual ~AnimationPoseNode() = default;

  virtual void increaseCurrentTime(float delta,
    const AnimationStatesMachineVariables& variablesSet) = 0;

  [[nodiscard]] virtual const SkeletalAnimationPose& getCurrentPose() const = 0;
};
