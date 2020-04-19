#pragma once

#include "Skeleton.h"
#include "AnimationClip.h"
#include "AnimationPose.h"

enum class AnimationClipEndBehaviour {
  Repeat, Stop
};
enum class AnimationClipState {
  NotStarted, Active, Paused, Finished
};

enum class AnimationClipTimeIncrementResult {
  Continued, Finished, Ignored
};

class AnimationClipInstance {
 public:
  AnimationClipInstance(std::shared_ptr<Skeleton> skeleton,
    std::shared_ptr<AnimationClip> animationClip);

  [[nodiscard]] const AnimationClip& getAnimationClip() const;
  [[nodiscard]] AnimationClip& getAnimationClip();

  [[nodiscard]] const AnimationPose& getAnimationPose() const;
  [[nodiscard]] const Skeleton& getSkeleton() const;

  [[nodiscard]] std::shared_ptr<Skeleton> getSkeletonPtr() const;

  void resetAnimationPoseCache();

  [[nodiscard]] AnimationClipTimeIncrementResult increaseCurrentTime(float delta);
  void resetClip();

  [[nodiscard]] float getCurrentTime() const;

  void setScale(float scale);
  [[nodiscard]] float getScale() const;

  void start();
  void pause();

  void setEndBehaviour(AnimationClipEndBehaviour behaviour);
  AnimationClipEndBehaviour getEndBehaviour() const;

  AnimationClipState getCurrentState() const;

 private:
  std::shared_ptr<Skeleton> m_skeleton;

  std::shared_ptr<AnimationClip> m_animationClip;
  mutable AnimationPose m_animationPose;
  mutable bool m_isAnimationPoseOutdated = true;

  float m_scale = 1.0f;
  float m_currentTime = 0.0f;

  AnimationClipEndBehaviour m_endBehaviour = AnimationClipEndBehaviour::Stop;
  AnimationClipState m_clipState = AnimationClipState::NotStarted;
};