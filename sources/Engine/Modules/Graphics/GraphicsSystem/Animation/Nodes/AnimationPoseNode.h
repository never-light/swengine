#pragma once

#include <Modules/Graphics/GraphicsSystem/Animation/AnimationClip.h>
#include <Modules/Graphics/GraphicsSystem/Animation/AnimationStatesMachineVariables.h>
#include <Modules/Graphics/GraphicsSystem/Animation/AnimationClipInstance.h>
#include <Modules/Graphics/GraphicsSystem/Animation/AnimationPose.h>

enum class AnimationPoseNodeState {
  NotStarted, Active, Paused, Finished
};

enum class AnimationPoseNodeFinalAction {
  Repeat, Stop
};

class AnimationPoseNode {
 public:
  AnimationPoseNode() = default;
  virtual ~AnimationPoseNode() = default;

  virtual void increaseCurrentTime(float delta,
    const AnimationStatesMachineVariables& variablesSet) = 0;

  [[nodiscard]] virtual const AnimationPose& getCurrentPose() const = 0;

  virtual void startAnimation() = 0;
  virtual void pauseAnimation() = 0;
  virtual void resetAnimation() = 0;

  [[nodiscard]] virtual AnimationPoseNodeState getState() const = 0;

  virtual void setFinalAction(AnimationPoseNodeFinalAction action) = 0;
  [[nodiscard]] virtual AnimationPoseNodeFinalAction getFinalAction() const = 0;
};
