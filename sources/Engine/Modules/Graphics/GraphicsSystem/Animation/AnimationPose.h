#pragma once

#include "Skeleton.h"
#include "Bone.h"
#include "AnimationClip.h"

class AnimationPose {
 public:
  explicit AnimationPose(std::shared_ptr<Skeleton> skeleton);

  AnimationPose(std::shared_ptr<Skeleton> skeleton,
    const std::vector<BonePose>& bonesPoses);

  void setBoneLocalPose(uint8_t boneIndex, const BonePose& pose);
  [[nodiscard]] const BonePose& getBoneLocalPose(uint8_t boneIndex) const;

  [[nodiscard]] const AnimationMatrixPalette& getMatrixPalette() const;

 private:
  std::shared_ptr<Skeleton> m_skeleton;

  std::vector<BonePose> m_bonesLocalPoses;

  mutable AnimationMatrixPalette m_matrixPalette;
  mutable bool m_isMatrixPaletteOutdated = true;

 private:
  friend class SkeletalAnimationClipInstance;
};

