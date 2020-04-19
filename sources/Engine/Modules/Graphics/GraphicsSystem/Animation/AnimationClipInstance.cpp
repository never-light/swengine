#include "precompiled.h"

#pragma hdrstop

#include <Exceptions/exceptions.h>
#include "AnimationClipInstance.h"
#include "SkeletalAnimationPose.h"

SkeletalAnimationClipInstance::SkeletalAnimationClipInstance(std::shared_ptr<Skeleton> skeleton,
  std::shared_ptr<AnimationClip> animationClip)
  : m_skeleton(skeleton),
    m_animationClip(animationClip),
    m_animationPose(skeleton, std::vector<BonePose>(skeleton->getBonesCount()))
{

}

const Skeleton& SkeletalAnimationClipInstance::getSkeleton() const
{
  return *m_skeleton.get();
}

std::shared_ptr<Skeleton> SkeletalAnimationClipInstance::getSkeletonPtr() const
{
  return m_skeleton;
}

void SkeletalAnimationClipInstance::resetAnimationPoseCache()
{
  m_isAnimationPoseOutdated = true;
}

void SkeletalAnimationClipInstance::increaseCurrentTime(float delta)
{
  if (m_clipState == SkeletalAnimationClipState::Paused) {
    return;
  }

  m_currentTime += delta * m_animationClip->getRate() * m_scale;

  if (m_currentTime > m_animationClip->getDuration()) {
    if (m_endBehaviour == SkeletalAnimationClipEndBehaviour::Stop) {
      m_currentTime = m_animationClip->getDuration();
      m_clipState = SkeletalAnimationClipState::Paused;
    }
    else if (m_endBehaviour == SkeletalAnimationClipEndBehaviour::StopAndReset) {
      m_currentTime = 0.0f;
      m_clipState = SkeletalAnimationClipState::Paused;
    }
    else if (m_endBehaviour == SkeletalAnimationClipEndBehaviour::Repeat) {
      int overflowParts = static_cast<int>(m_currentTime / m_animationClip->getDuration());
      m_currentTime -= m_animationClip->getDuration() * static_cast<float>(overflowParts);
    }
    else {
      THROW_EXCEPTION(NotImplementedException, "Clip behaviour is not impelented");
    }
  }

  resetAnimationPoseCache();
}

void SkeletalAnimationClipInstance::resetCurrentTime()
{
  m_currentTime = 0.0f;
}

float SkeletalAnimationClipInstance::getCurrentTime() const
{
  return m_currentTime;
}

void SkeletalAnimationClipInstance::setScale(float scale)
{
  m_scale = scale;
}

float SkeletalAnimationClipInstance::getScale() const
{
  return m_scale;
}

void SkeletalAnimationClipInstance::start()
{
  m_clipState = SkeletalAnimationClipState::Active;
}

void SkeletalAnimationClipInstance::pause()
{
  m_clipState = SkeletalAnimationClipState::Paused;
}

void SkeletalAnimationClipInstance::setEndBehaviour(SkeletalAnimationClipEndBehaviour behaviour)
{
  m_endBehaviour = behaviour;
}

const AnimationClip& SkeletalAnimationClipInstance::getAnimationClip() const
{
  return *m_animationClip.get();
}

AnimationClip& SkeletalAnimationClipInstance::getAnimationClip()
{
  return *m_animationClip.get();
}

const SkeletalAnimationPose& SkeletalAnimationClipInstance::getAnimationPose() const
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