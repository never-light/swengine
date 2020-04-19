#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationPose.h"

SkeletalAnimationPose::SkeletalAnimationPose(std::shared_ptr<Skeleton> skeleton)
  : m_skeleton(skeleton),
    m_bonesLocalPoses(std::vector<BonePose>(skeleton->getBonesCount())),
    m_matrixPalette(std::vector<glm::mat4>(skeleton->getBonesCount(), glm::identity<glm::mat4>()))
{

}

SkeletalAnimationPose::SkeletalAnimationPose(std::shared_ptr<Skeleton> skeleton,
  const std::vector<BonePose>& bonesPoses)
  : m_skeleton(skeleton),
    m_bonesLocalPoses(bonesPoses),
    m_matrixPalette(std::vector<glm::mat4>(bonesPoses.size(), glm::identity<glm::mat4>()))
{

}

void SkeletalAnimationPose::setBoneLocalPose(uint8_t boneIndex, const BonePose& pose)
{
    m_bonesLocalPoses[boneIndex] = pose;
    m_isMatrixPaletteOutdated = true;
}

const BonePose& SkeletalAnimationPose::getBoneLocalPose(uint8_t boneIndex) const
{
    return m_bonesLocalPoses[boneIndex];
}

const AnimationMatrixPalette& SkeletalAnimationPose::getMatrixPalette() const
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