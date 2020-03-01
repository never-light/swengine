#pragma once

#include "SkeletalAnimationStatesManager.h"

class SkeletalAnimationComponent
{
public:
    SkeletalAnimationComponent(std::shared_ptr<Skeleton> skeleton);

    SkeletalAnimationStatesMachine& getAnimationStatesMachine();
    const SkeletalAnimationStatesMachine& getAnimationStatesMachine() const;

    const SkeletalAnimationMatrixPalette& getMatrixPalette() const;

private:
    SkeletalAnimationStatesMachine m_animationStatesMachine;
};

