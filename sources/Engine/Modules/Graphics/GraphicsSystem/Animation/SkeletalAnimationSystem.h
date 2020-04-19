#pragma once

#include <Modules/ECS/ECS.h>
#include <Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Modules/Graphics/GraphicsSystem/TransformComponent.h>

#include "SkeletalAnimationComponent.h"

class SkeletalAnimationSystem : public GameSystem {
 public:
  SkeletalAnimationSystem();
  ~SkeletalAnimationSystem() override;

  void configure(GameWorld* gameWorld) override;
  void unconfigure(GameWorld* gameWorld) override;

  void update(GameWorld* gameWorld, float delta) override;

 private:
  void updateAnimationStateMachine(AnimationStatesMachine& stateMachine, float delta);
  void updateObjectBounds(TransformComponent& transformComponent,
    SkeletalAnimationComponent& skeletalAnimationComponent,
    MeshRendererComponent& meshRendererComponent,
    float delta);
};
