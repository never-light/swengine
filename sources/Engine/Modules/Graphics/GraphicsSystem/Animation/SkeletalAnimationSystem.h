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
  void updateAnimationStateMachine(AnimationStatesMachine& stateMachine, float delta);
  void updateObjectBounds(TransformComponent& transformComponent,
    SkeletalAnimationComponent& skeletalAnimationComponent,
    MeshRendererComponent& meshRendererComponent,
    float delta);
};
