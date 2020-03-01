#include "precompiled.h"
#pragma hdrstop

#include "SkeletalAnimationComponent.h"

SkeletalAnimationComponent::SkeletalAnimationComponent(std::shared_ptr<Skeleton> skeleton)
    : m_animationStatesMachine(skeleton)
{

}

SkeletalAnimationStatesMachine& SkeletalAnimationComponent::getAnimationStatesMachine()
{
    return m_animationStatesMachine;
}

const SkeletalAnimationStatesMachine& SkeletalAnimationComponent::getAnimationStatesMachine() const
{
    return m_animationStatesMachine;
}

const SkeletalAnimationMatrixPalette& SkeletalAnimationComponent::getMatrixPalette() const
{
    return m_animationStatesMachine.getCurrentMatrixPalette();
}
