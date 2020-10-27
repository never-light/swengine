#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationComponent.h"

SkeletalAnimationComponent::SkeletalAnimationComponent(
  ResourceHandle<AnimationStatesMachine> animationStatesMachine)
  : m_animationStatesMachine(animationStatesMachine)
{

}

AnimationStatesMachine& SkeletalAnimationComponent::getAnimationStatesMachineRef()
{
  return *m_animationStatesMachine;
}

const AnimationStatesMachine& SkeletalAnimationComponent::getAnimationStatesMachineRef() const
{
  return *m_animationStatesMachine;
}

const AnimationMatrixPalette& SkeletalAnimationComponent::getMatrixPalette() const
{
  return m_animationStatesMachine->getCurrentMatrixPalette();
}

void SkeletalAnimationComponent::setAnimationStatesMachine(ResourceHandle<AnimationStatesMachine> statesMachine)
{
  m_animationStatesMachine = statesMachine;
}

ResourceHandle<AnimationStatesMachine> SkeletalAnimationComponent::getAnimationStatesMachine() const
{
  return m_animationStatesMachine;
}
