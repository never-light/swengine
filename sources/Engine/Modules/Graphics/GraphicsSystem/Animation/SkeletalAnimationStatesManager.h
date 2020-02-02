#pragma once

#include <unordered_map>
#include "SkeletalAnimationClip.h"

class SkeletalAnimationStatesManager
{
public:
    SkeletalAnimationStatesManager(std::shared_ptr<Skeleton> skeleton);
    ~SkeletalAnimationStatesManager() = default;

    void addAnimationClipInstance(const SkeletalAnimationClipInstance& clip);

    bool hasActiveClip() const;
    void resetActiveClip();

    void setActiveClip(const std::string& name);

    const SkeletalAnimationClipInstance& getActiveClip() const;
    SkeletalAnimationClipInstance& getActiveClip();

    const SkeletalAnimationPose& getCurrentPose() const;
    const SkeletalAnimationMatrixPalette& getCurrentMatrixPalette() const;

private:
    std::shared_ptr<Skeleton> m_skeleton;
    std::unordered_map<std::string, SkeletalAnimationClipInstance> m_animationClips;

    SkeletalAnimationClipInstance* m_activeClip = nullptr;
};

