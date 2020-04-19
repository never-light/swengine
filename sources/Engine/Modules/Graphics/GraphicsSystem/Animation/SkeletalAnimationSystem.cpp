#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationSystem.h"
#include "Bone.h"

SkeletalAnimationSystem::SkeletalAnimationSystem()
{

}

SkeletalAnimationSystem::~SkeletalAnimationSystem()
{

}

void SkeletalAnimationSystem::configure(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);
}

void SkeletalAnimationSystem::unconfigure(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);
}

void SkeletalAnimationSystem::update(GameWorld* gameWorld, float delta)
{
  for (const GameObject* obj : gameWorld->allWith<SkeletalAnimationComponent>()) {
    auto& animationComponent = obj->getComponent<SkeletalAnimationComponent>().getRef();
    auto& statesMachine = animationComponent.getAnimationStatesMachine();

    updateAnimationStateMachine(statesMachine, delta);

    if (obj->hasComponent<MeshRendererComponent>()) {
      updateObjectBounds(obj->getComponent<TransformComponent>().getRef(),
        animationComponent,
        obj->getComponent<MeshRendererComponent>().getRef(),
        delta);
    }
  }
}

void SkeletalAnimationSystem::updateAnimationStateMachine(AnimationStatesMachine& stateMachine, float delta)
{
  stateMachine.increaseCurrentTime(delta);
}

void SkeletalAnimationSystem::updateObjectBounds(TransformComponent& transformComponent,
  SkeletalAnimationComponent& skeletalAnimationComponent,
  MeshRendererComponent& meshRendererComponent,
  float delta)
{
  glm::mat4 boundTransformation = transformComponent.getTransform()->getTransformationMatrix() *
    skeletalAnimationComponent.getMatrixPalette().bonesTransforms[0];

  meshRendererComponent.updateBounds(boundTransformation);
}
