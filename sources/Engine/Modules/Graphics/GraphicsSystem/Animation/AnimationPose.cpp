#include "precompiled.h"

#pragma hdrstop

#include "AnimationPose.h"

AnimationPose::AnimationPose(ResourceHandle<Skeleton> skeleton)
  : m_skeleton(skeleton),
    m_bonesLocalPoses(std::vector<BonePose>(skeleton->getBonesCount())),
    m_matrixPalette(std::vector<glm::mat4>(skeleton->getBonesCount(), glm::identity<glm::mat4>()))
{

}

AnimationPose::AnimationPose(ResourceHandle<Skeleton> skeleton,
  const std::vector<BonePose>& bonesPoses)
  : m_skeleton(skeleton),
    m_bonesLocalPoses(bonesPoses),
    m_matrixPalette(std::vector<glm::mat4>(bonesPoses.size(), glm::identity<glm::mat4>()))
{

}

void AnimationPose::setBoneLocalPose(uint8_t boneIndex, const BonePose& pose)
{
  m_bonesLocalPoses[boneIndex] = pose;
  m_isMatrixPaletteOutdated = true;
}

const BonePose& AnimationPose::getBoneLocalPose(uint8_t boneIndex) const
{
  return m_bonesLocalPoses[boneIndex];
}

const AnimationMatrixPalette& AnimationPose::getMatrixPalette() const
{
  if (m_isMatrixPaletteOutdated) {
    // Update matrix palette hierarchy
    m_matrixPalette.bonesTransforms[0] = m_bonesLocalPoses[0].getBoneMatrix();

    for (uint8_t boneIndex = 1; boneIndex < m_matrixPalette.bonesTransforms.size(); boneIndex++) {
      m_matrixPalette.bonesTransforms[boneIndex] =
        m_matrixPalette.bonesTransforms[m_skeleton->getBoneParentId(boneIndex)] *
          m_bonesLocalPoses[boneIndex].getBoneMatrix();
    }

    // Multiply matrix palette by inverse bind pose matrices
    for (uint8_t boneIndex = 0; boneIndex < m_matrixPalette.bonesTransforms.size(); boneIndex++) {
      m_matrixPalette.bonesTransforms[boneIndex] *= m_skeleton->getBone(boneIndex).getInverseBindPoseMatrix();
    }

    m_isMatrixPaletteOutdated = false;
  }

  return m_matrixPalette;
}

uint8_t AnimationPose::getBonesCount() const
{
  return m_skeleton->getBonesCount();
}

void AnimationPose::interpolate(const AnimationPose& first,
  const AnimationPose& second,
  float factor,
  AnimationPose& result)
{
  SW_ASSERT(first.getBonesCount() == second.getBonesCount());

  uint8_t bonesCount = first.getBonesCount();

  for (uint8_t boneIndex = 0; boneIndex < bonesCount; boneIndex++) {
    BonePose interpolatedBonePose = BonePose::interpolate(first.getBoneLocalPose(boneIndex),
      second.getBoneLocalPose(boneIndex), factor);

    result.setBoneLocalPose(boneIndex, interpolatedBonePose);
  }
}

void AnimationPose::interpolate(const AnimationPose& first,
  const AnimationPose& second, float factor, const std::vector<uint8_t> affectedBonesMask, AnimationPose& result)
{
  SW_ASSERT(first.getBonesCount() == second.getBonesCount());

  uint8_t bonesCount = first.getBonesCount();

  for (uint8_t boneIndex = 0; boneIndex < bonesCount; boneIndex++) {
    if (affectedBonesMask[boneIndex]) {
      BonePose interpolatedBonePose = BonePose::interpolate(first.getBoneLocalPose(boneIndex),
        second.getBoneLocalPose(boneIndex), factor);

      result.setBoneLocalPose(boneIndex, interpolatedBonePose);
    }
    else {
      result.setBoneLocalPose(boneIndex, first.getBoneLocalPose(boneIndex));
    }
  }

}

const Skeleton* AnimationPose::getSkeleton() const
{
  return m_skeleton.get();
}

Skeleton* AnimationPose::getSkeleton()
{
  return m_skeleton.get();
}
