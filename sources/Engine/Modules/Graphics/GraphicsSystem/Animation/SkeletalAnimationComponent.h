#pragma once

#include <memory>

#include "AnimationStatesMachine.h"

class SkeletalAnimationComponent {
 public:
  explicit SkeletalAnimationComponent(ResourceHandle<AnimationStatesMachine> animationStatesMachine);

  void setAnimationStatesMachine(ResourceHandle<AnimationStatesMachine> statesMachine);
  [[nodiscard]] ResourceHandle<AnimationStatesMachine> getAnimationStatesMachine() const;

  AnimationStatesMachine& getAnimationStatesMachineRef();
  [[nodiscard]] const AnimationStatesMachine& getAnimationStatesMachineRef() const;

  [[nodiscard]] const AnimationMatrixPalette& getMatrixPalette() const;

 private:
  ResourceHandle<AnimationStatesMachine> m_animationStatesMachine;
};

