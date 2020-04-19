#pragma once

#include <memory>

#include "AnimationStatesMachine.h"

class SkeletalAnimationComponent {
 public:
  explicit SkeletalAnimationComponent(std::shared_ptr<Skeleton> skeleton);

  AnimationStatesMachine& getAnimationStatesMachine();
  [[nodiscard]] const AnimationStatesMachine& getAnimationStatesMachine() const;

  [[nodiscard]] const AnimationMatrixPalette& getMatrixPalette() const;

 private:
  AnimationStatesMachine m_animationStatesMachine;
};

