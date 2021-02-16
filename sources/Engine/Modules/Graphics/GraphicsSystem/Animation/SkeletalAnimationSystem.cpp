#include "precompiled.h"

#pragma hdrstop

#include "Modules/Graphics/GraphicsSystem/DebugPainter.h"
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
    if (obj.getComponent<TransformComponent>()->isOnline()) {
      auto animationComponent = obj.getComponent<SkeletalAnimationComponent>();
      auto& statesMachine = animationComponent->getAnimationStatesMachineRef();

      if (statesMachine.isActive()) {
        updateAnimationStateMachine(statesMachine, delta);

        if (obj.hasComponent<MeshRendererComponent>()) {
          updateObjectBounds(*obj.getComponent<TransformComponent>().get(),
            *animationComponent.get(),
            delta);
        }

        const auto& palette = statesMachine.getCurrentMatrixPalette();
        const auto& skeleton = statesMachine.getSkeleton();

        for (size_t boneIndex = 0; boneIndex < palette.bonesTransforms.size(); boneIndex++) {
          glm::vec4 boneCenter = glm::inverse(skeleton->getBone(uint8_t(boneIndex)).getInverseBindPoseMatrix()) *
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

          glm::vec4 parentBoneCenter;
          auto parentBoneIndex = skeleton->getBoneParentId(uint8_t(boneIndex));

          if (parentBoneIndex == Bone::ROOT_BONE_PARENT_ID) {
            parentBoneCenter = boneCenter;
            parentBoneIndex = uint8_t(boneIndex);
          }
          else {
            parentBoneCenter =
              glm::inverse(skeleton->getBone(parentBoneIndex).getInverseBindPoseMatrix())
                * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
          }

          boneCenter = palette.bonesTransforms[boneIndex] * boneCenter;
          parentBoneCenter = palette.bonesTransforms[parentBoneIndex] * parentBoneCenter;

          glm::vec3 boneVectorStart = obj.getComponent<TransformComponent>()->getTransform().getTransformationMatrix()
            * obj.getComponent<MeshRendererComponent>()->getMeshInstance()->getInverseSceneTransform() * boneCenter;
          glm::vec3 boneVectorEnd = obj.getComponent<TransformComponent>()->getTransform().getTransformationMatrix()
            * obj.getComponent<MeshRendererComponent>()->getMeshInstance()->getInverseSceneTransform() * parentBoneCenter;

          boneVectorStart += glm::vec3(2.0f, 0.0f, 0.0f);
          boneVectorEnd += glm::vec3(2.0f, 0.0f, 0.0f);

          DebugPainter::renderSphere(boneVectorStart, 0.03f);
          DebugPainter::renderSegment(boneVectorStart, boneVectorEnd, { 0.0f, 0.8f, 0.0f, 1.0f });
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
