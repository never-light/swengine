#include "precompiled.h"
#pragma hdrstop

#include "SkeletalAnimationComponent.h"

SkeletalAnimationComponent::SkeletalAnimationComponent(std::shared_ptr<Skeleton> skeleton)
    : m_animationStatesManager(skeleton)
{

}

SkeletalAnimationStatesManager& SkeletalAnimationComponent::getAnimationStatesManager()
{
    return m_animationStatesManager;
}

const SkeletalAnimationStatesManager& SkeletalAnimationComponent::getAnimationStatesManager() const
{
    return m_animationStatesManager;
}

bool SkeletalAnimationComponent::hasActiveAnimation() const
{
    return m_animationStatesManager.hasActiveClip();
}

const SkeletalAnimationMatrixPalette& SkeletalAnimationComponent::getMatrixPalette() const
{
    return m_animationStatesManager.getActiveClip().getAnimationPose().getMatrixPalette();
}
