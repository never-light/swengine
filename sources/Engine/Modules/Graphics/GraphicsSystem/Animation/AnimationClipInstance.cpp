#include "precompiled.h"

#pragma hdrstop

#include <Exceptions/exceptions.h>
#include "AnimationClipInstance.h"
#include "AnimationPose.h"

AnimationClipInstance::AnimationClipInstance(std::shared_ptr<Skeleton> skeleton,
  std::shared_ptr<AnimationClip> animationClip)
  : m_skeleton(skeleton),
    m_animationClip(animationClip),
    m_animationPose(skeleton, std::vector<BonePose>(skeleton->getBonesCount()))
{

}

const Skeleton& AnimationClipInstance::getSkeleton() const
{
  return *m_skeleton.get();
}

std::shared_ptr<Skeleton> AnimationClipInstance::getSkeletonPtr() const
{
  return m_skeleton;
}

void AnimationClipInstance::resetAnimationPoseCache()
{
  m_isAnimationPoseOutdated = true;
}

AnimationClipTimeIncrementResult AnimationClipInstance::increaseCurrentTime(float delta)
{
  if (m_clipState != AnimationClipState::Active) {
    return AnimationClipTimeIncrementResult::Ignored;
  }

  AnimationClipTimeIncrementResult incrementResult = AnimationClipTimeIncrementResult::Continued;

  m_currentTime += delta * m_animationClip->getRate() * m_scale;

  if (m_currentTime > m_animationClip->getDuration()) {
    if (m_endBehaviour == AnimationClipEndBehaviour::Stop) {
      m_currentTime = 0.0f;
      m_clipState = AnimationClipState::Finished;
      incrementResult = AnimationClipTimeIncrementResult::Finished;
    }
    else if (m_endBehaviour == AnimationClipEndBehaviour::Repeat) {
      int overflowParts = static_cast<int>(m_currentTime / m_animationClip->getDuration());
      m_currentTime -= m_animationClip->getDuration() * static_cast<float>(overflowParts);
      incrementResult = AnimationClipTimeIncrementResult::Continued;
    }
    else {
      SW_ASSERT(false);
    }
  }

  resetAnimationPoseCache();

  return incrementResult;
}

void AnimationClipInstance::resetClip()
{
  m_currentTime = 0.0f;
  m_clipState = AnimationClipState::NotStarted;
}

float AnimationClipInstance::getCurrentTime() const
{
  return m_currentTime;
}

void AnimationClipInstance::setScale(float scale)
{
  m_scale = scale;
}

float AnimationClipInstance::getScale() const
{
  return m_scale;
}

void AnimationClipInstance::start()
{
  m_clipState = AnimationClipState::Active;
}

void AnimationClipInstance::pause()
{
  m_clipState = AnimationClipState::Paused;
}

void AnimationClipInstance::setEndBehaviour(AnimationClipEndBehaviour behaviour)
{
  m_endBehaviour = behaviour;
}

const AnimationClip& AnimationClipInstance::getAnimationClip() const
{
  return *m_animationClip.get();
}

AnimationClip& AnimationClipInstance::getAnimationClip()
{
  return *m_animationClip.get();
}

const AnimationPose& AnimationClipInstance::getAnimationPose() const
{
  if (!m_isAnimationPoseOutdated) {
    return m_animationPose;
  }

  // Update animation pose
  m_animationPose.setBoneLocalPose(0, m_animationClip->getBoneRelativePose(0, m_currentTime));

  for (uint8_t boneIndex = 1; boneIndex < m_skeleton->getBonesCount(); boneIndex++) {
    BonePose currentLocalPose = m_animationClip->getBoneRelativePose(boneIndex, m_currentTime);
    m_animationPose.setBoneLocalPose(boneIndex, currentLocalPose);
  }

  m_isAnimationPoseOutdated = false;
  return m_animationPose;
}

AnimationClipEndBehaviour AnimationClipInstance::getEndBehaviour() const
{
  return m_endBehaviour;
}

AnimationClipState AnimationClipInstance::getCurrentState() const
{
  return m_clipState;
}
