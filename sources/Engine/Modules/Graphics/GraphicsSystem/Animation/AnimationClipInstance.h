#pragma once

#include "Skeleton.h"
#include "AnimationClip.h"
#include "SkeletalAnimationPose.h"

enum class SkeletalAnimationClipEndBehaviour {
  Repeat, Stop, StopAndReset
};
enum class SkeletalAnimationClipState {
  Active, Paused
};

class SkeletalAnimationClipInstance {
 public:
  SkeletalAnimationClipInstance(std::shared_ptr<Skeleton> skeleton,
    std::shared_ptr<AnimationClip> animationClip);

  [[nodiscard]] const AnimationClip& getAnimationClip() const;
  [[nodiscard]] AnimationClip& getAnimationClip();

  [[nodiscard]] const SkeletalAnimationPose& getAnimationPose() const;
  [[nodiscard]] const Skeleton& getSkeleton() const;

  [[nodiscard]] std::shared_ptr<Skeleton> getSkeletonPtr() const;

  void resetAnimationPoseCache();

  void increaseCurrentTime(float delta);
  void resetCurrentTime();

  [[nodiscard]] float getCurrentTime() const;

  void setScale(float scale);
  [[nodiscard]] float getScale() const;

  void start();
  void pause();

  void setEndBehaviour(SkeletalAnimationClipEndBehaviour behaviour);

 private:
  std::shared_ptr<Skeleton> m_skeleton;

  std::shared_ptr<AnimationClip> m_animationClip;
  mutable SkeletalAnimationPose m_animationPose;
  mutable bool m_isAnimationPoseOutdated = true;

  float m_scale = 1.0f;
  float m_currentTime = 0.0f;

  SkeletalAnimationClipEndBehaviour m_endBehaviour = SkeletalAnimationClipEndBehaviour::Stop;
  SkeletalAnimationClipState m_clipState = SkeletalAnimationClipState::Active;
};