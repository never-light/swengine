#pragma once

#include <memory>

#include "SkeletalAnimationStatesManager.h"

class SkeletalAnimationComponent {
 public:
  explicit SkeletalAnimationComponent(std::shared_ptr<Skeleton> skeleton);

  SkeletalAnimationStatesMachine& getAnimationStatesMachine();
  [[nodiscard]] const SkeletalAnimationStatesMachine& getAnimationStatesMachine() const;

  [[nodiscard]] const SkeletalAnimationMatrixPalette& getMatrixPalette() const;

 private:
  SkeletalAnimationStatesMachine m_animationStatesMachine;
};

