#pragma once

#include "SkeletalAnimationStatesManager.h"

class SkeletalAnimationComponent
{
public:
    SkeletalAnimationComponent(std::shared_ptr<Skeleton> skeleton);

    SkeletalAnimationStatesManager& getAnimationStatesManager();
    const SkeletalAnimationStatesManager& getAnimationStatesManager() const;

    bool hasActiveAnimation() const;
    const SkeletalAnimationMatrixPalette& getMatrixPalette() const;

private:
    SkeletalAnimationStatesManager m_animationStatesManager;
};

