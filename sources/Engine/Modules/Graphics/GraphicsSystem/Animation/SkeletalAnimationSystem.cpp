#include "precompiled.h"

#pragma hdrstop

#include "Modules/Graphics/GraphicsSystem/DebugPainter.h"
#include "Modules/Graphics/GraphicsSystem/GraphicsScene.h"
#include "SkeletalAnimationSystem.h"
#include "Bone.h"

SkeletalAnimationSystem::SkeletalAnimationSystem() = default;

SkeletalAnimationSystem::~SkeletalAnimationSystem() = default;

void SkeletalAnimationSystem::configure()
{
}

void SkeletalAnimationSystem::unconfigure()
{
}

void SkeletalAnimationSystem::update(float delta)
{
  for (GameObject obj : getGameWorld()->allWith<SkeletalAnimationComponent>()) {
    if (obj.getComponent<TransformComponent>()->isOnline() && obj.hasComponent<ObjectSceneNodeComponent>()) {
      auto animationComponent = obj.getComponent<SkeletalAnimationComponent>();
      auto& statesMachine = animationComponent->getAnimationStatesMachineRef();

      if (statesMachine.isActive()) {
        updateAnimationStateMachine(statesMachine, delta);

        if (obj.hasComponent<MeshRendererComponent>()) {
          updateObjectBounds(*obj.getComponent<TransformComponent>().get(),
            *animationComponent.get(),
            delta);
        }
      }
    }
  }
}

void SkeletalAnimationSystem::updateAnimationStateMachine(AnimationStatesMachine& stateMachine, float delta)
{
  stateMachine.increaseCurrentTime(delta);
}

void SkeletalAnimationSystem::updateObjectBounds(TransformComponent& transformComponent,
  SkeletalAnimationComponent& skeletalAnimationComponent,
  float delta)
{
  ARG_UNUSED(delta);
  ARG_UNUSED(transformComponent);
  ARG_UNUSED(skeletalAnimationComponent);

  // TODO: track some bone as marker and use it to align bounding mesh properly.
  //  The method below is not precise and, so, not appropriate.
  //  Or just precompute bounding volumes data for each animation frame.

//  glm::mat4 boundTransformation = transformComponent.getTransform().getTransformationMatrix() *
//    skeletalAnimationComponent.getMatrixPalette().bonesTransforms[0]
//    * skeletalAnimationComponent.getAnimationStatesMachine()->getSkeleton()->getBone(0).getInverseBindPoseMatrix();
//
//  transformComponent.updateBounds(boundTransformation);
}
