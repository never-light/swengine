#include "precompiled.h"
#pragma hdrstop

#include "SkeletalAnimationStatesManager.h"

SkeletalAnimationStatesManager::SkeletalAnimationStatesManager(std::shared_ptr<Skeleton> skeleton)
    : m_skeleton(skeleton)
{

}

void SkeletalAnimationStatesManager::addAnimationClipInstance(const SkeletalAnimationClipInstance& clip)
{
    SW_ASSERT(m_skeleton.get() == &clip.getAnimationClip().getSkeleton());

    m_animationClips.emplace(clip.getAnimationClip().getName(), clip);
}

bool SkeletalAnimationStatesManager::hasActiveClip() const
{
    return m_activeClip != nullptr;
}

void SkeletalAnimationStatesManager::resetActiveClip()
{
    m_activeClip = nullptr;
}

void SkeletalAnimationStatesManager::setActiveClip(const std::string& name)
{
    m_activeClip = &m_animationClips.at(name);
}

const SkeletalAnimationClipInstance& SkeletalAnimationStatesManager::getActiveClip() const
{
    return *m_activeClip;
}

SkeletalAnimationClipInstance& SkeletalAnimationStatesManager::getActiveClip()
{
    return *m_activeClip;
}

const SkeletalAnimationPose& SkeletalAnimationStatesManager::getCurrentPose() const
{
    return m_activeClip->getAnimationPose();
}

const SkeletalAnimationMatrixPalette& SkeletalAnimationStatesManager::getCurrentMatrixPalette() const
{
    return m_activeClip->getAnimationPose().getMatrixPalette();
}
