#include "precompiled.h"

#pragma hdrstop

#include "BoneAnimationChannel.h"

BoneAnimationChannel::BoneAnimationChannel(const std::vector<BoneAnimationPositionFrame>& positionFrames,
  const std::vector<BoneAnimationOrientationFrame>& orientationFrames)
  : m_positionFrames(positionFrames),
    m_orientationFrames(orientationFrames)
{

}

std::vector<BoneAnimationPositionFrame>& BoneAnimationChannel::getPositionFrames()
{
  return m_positionFrames;
}

const std::vector<BoneAnimationPositionFrame>& BoneAnimationChannel::getPositionFrames() const
{
  return m_positionFrames;
}

const std::vector<BoneAnimationOrientationFrame>& BoneAnimationChannel::getOrientationFrames() const
{
  return m_orientationFrames;
}

std::vector<BoneAnimationOrientationFrame>& BoneAnimationChannel::getOrientationFrames()
{
  return m_orientationFrames;
}