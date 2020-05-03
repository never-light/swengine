#pragma once

#include <memory>

#include "AnimationStatesMachine.h"

class SkeletalAnimationComponent {
 public:
  explicit SkeletalAnimationComponent(std::shared_ptr<Skeleton> skeleton);

  void setAnimationStatesMachine(std::shared_ptr<AnimationStatesMachine> statesMachine);
  [[nodiscard]] std::shared_ptr<AnimationStatesMachine> getAnimationStatesMachine() const;

  AnimationStatesMachine& getAnimationStatesMachineRef();
  [[nodiscard]] const AnimationStatesMachine& getAnimationStatesMachineRef() const;

  [[nodiscard]] const AnimationMatrixPalette& getMatrixPalette() const;

 private:
  std::shared_ptr<AnimationStatesMachine> m_animationStatesMachine;
};

