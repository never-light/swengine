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

  [[nodiscrd]] uint8_t getBonesCount() const;

 public:
  static void interpolate(const AnimationPose& first,
    const AnimationPose& second, float factor, AnimationPose& result);

  static void interpolate(const AnimationPose& first,
    const AnimationPose& second, float factor, const std::vector<uint8_t> affectedBonesMask, AnimationPose& result);

 private:
  std::shared_ptr<Skeleton> m_skeleton;

  std::vector<BonePose> m_bonesLocalPoses;

  mutable AnimationMatrixPalette m_matrixPalette;
  mutable bool m_isMatrixPaletteOutdated = true;

 private:
  friend class SkeletalAnimationClipInstance;
};

