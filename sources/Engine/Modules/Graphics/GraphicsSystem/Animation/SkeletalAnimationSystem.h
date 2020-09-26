#pragma once

#include <Modules/ECS/ECS.h>
#include <Modules/Graphics/GraphicsSystem/MeshRendererComponent.h>
#include <Modules/Graphics/GraphicsSystem/TransformComponent.h>

#include "SkeletalAnimationComponent.h"

class SkeletalAnimationSystem : public GameSystem {
 public:
  SkeletalAnimationSystem();
  ~SkeletalAnimationSystem() override;

  void configure() override;
  void unconfigure() override;

  void update(float delta) override;

 private:
  static void updateAnimationStateMachine(AnimationStatesMachine& stateMachine, float delta);
  static void updateObjectBounds(TransformComponent& transformComponent,
    SkeletalAnimationComponent& skeletalAnimationComponent,
    float delta);
};
