#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationComponent.h"

SkeletalAnimationComponent::SkeletalAnimationComponent(std::shared_ptr<Skeleton> skeleton)
  : m_animationStatesMachine(std::make_shared<AnimationStatesMachine>(skeleton))
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

void SkeletalAnimationComponent::setAnimationStatesMachine(std::shared_ptr<AnimationStatesMachine> statesMachine)
{
  m_animationStatesMachine = statesMachine;
}

std::shared_ptr<AnimationStatesMachine> SkeletalAnimationComponent::getAnimationStatesMachine() const
{
  return m_animationStatesMachine;
}
