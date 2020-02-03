#pragma once

#include <Modules/ECS/ECS.h>

#include "SkeletalAnimationComponent.h"

class SkeletalAnimationSystem : public GameSystem {
public:
    SkeletalAnimationSystem();
    ~SkeletalAnimationSystem() override;

    void configure(GameWorld *gameWorld) override;
    void unconfigure(GameWorld *gameWorld) override;

    void update(GameWorld *gameWorld, float delta) override;

private:
    void updateAnimationState(SkeletalAnimationClipInstance& activeAnimationClip, float delta);
};
