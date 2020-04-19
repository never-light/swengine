#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationComponent.h"

SkeletalAnimationComponent::SkeletalAnimationComponent(std::shared_ptr<Skeleton> skeleton)
  : m_animationStatesMachine(skeleton)
{

}

AnimationStatesMachine& SkeletalAnimationComponent::getAnimationStatesMachine()
{
  return m_animationStatesMachine;
}

const AnimationStatesMachine& SkeletalAnimationComponent::getAnimationStatesMachine() const
{
  return m_animationStatesMachine;
}

const AnimationMatrixPalette& SkeletalAnimationComponent::getMatrixPalette() const
{
  return m_animationStatesMachine.getCurrentMatrixPalette();
}
