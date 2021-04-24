#include "precompiled.h"

#pragma hdrstop

#include "MeshRenderingSystem.h"

#include <utility>
#include <glm/gtx/string_cast.hpp>

#include "Modules/ECS/ECS.h"
#include "Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationComponent.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "DebugPainter.h"

MeshRenderingSystem::MeshRenderingSystem(
  std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<GraphicsScene> graphicsScene)
  : RenderingSystem(std::move(graphicsContext), std::move(graphicsScene))
{

}

MeshRenderingSystem::~MeshRenderingSystem() = default;

void MeshRenderingSystem::configure()
{
}

void MeshRenderingSystem::unconfigure()
{
}

void MeshRenderingSystem::update(float delta)
{
  ARG_UNUSED(delta);

  if (m_isSkeletonsRenderingEnabled) {
    for (GameObject object : getGameWorld()->allWith<SkeletalAnimationComponent>()) {
      if (object.getComponent<TransformComponent>()->isOnline() && object.hasComponent<ObjectSceneNodeComponent>()) {
        debugDrawSkeleton(object);
      }
    }
  }
}

void MeshRenderingSystem::render()
{
  // TODO: reorganize this logic and get rid of these static cache variables
  static std::vector<glm::mat4> skinnedMeshesPremultipliedTransforms;
  SW_ASSERT(skinnedMeshesPremultipliedTransforms.size() < 128);
  skinnedMeshesPremultipliedTransforms.reserve(128);
  skinnedMeshesPremultipliedTransforms.clear();

  static std::vector<GameObject> visibleObjects;
  SW_ASSERT(visibleObjects.size() < 1024);
  visibleObjects.reserve(1024);
  visibleObjects.clear();

  m_graphicsScene->queryVisibleObjects(visibleObjects);

  auto& frameStats = m_graphicsScene->getFrameStats();

  frameStats.increaseCulledSubMeshesCount(
    m_graphicsScene->getDrawableObjectsCount() - visibleObjects.size());

  for (GameObject obj : visibleObjects) {
    auto& transformComponent = *obj.getComponent<TransformComponent>().get();
    auto& transform = transformComponent.getTransform();

    auto meshComponent = obj.getComponent<MeshRendererComponent>();

    Mesh* mesh = meshComponent->getMeshInstance().get();
    SW_ASSERT(mesh != nullptr);

    const size_t subMeshesCount = mesh->getSubMeshesCount();
    SW_ASSERT(subMeshesCount != 0);

    frameStats.increaseSubMeshesCount(subMeshesCount);

    bool isMeshAnimated = mesh->isSkinned() && mesh->hasSkeleton() && obj.hasComponent<SkeletalAnimationComponent>();

    if (isMeshAnimated) {
      // TODO: investigate and debug getInverseSceneTransform behaviour, check
      //  that this multiplication is correct
      skinnedMeshesPremultipliedTransforms
        .push_back(transform.getTransformationMatrix() * mesh->getInverseSceneTransform());
    }

    for (size_t subMeshIndex = 0; subMeshIndex < subMeshesCount; subMeshIndex++) {

      const glm::mat4* matrixPalette = nullptr;

      if (isMeshAnimated) {
        auto& skeletalAnimationComponent = *obj.getComponent<SkeletalAnimationComponent>().get();

        if (skeletalAnimationComponent.getAnimationStatesMachineRef().isActive()) {

          const AnimationStatesMachine& animationStatesMachine =
            skeletalAnimationComponent.getAnimationStatesMachineRef();
          const AnimationMatrixPalette& currentMatrixPalette =
            animationStatesMachine.getCurrentMatrixPalette();

          matrixPalette = currentMatrixPalette.bonesTransforms.data();
        }
      }

      frameStats.increasePrimitivesCount(mesh->getSubMeshIndicesCount(subMeshIndex) / 3);

      m_graphicsContext->scheduleRenderTask(RenderTask{
        .material = meshComponent->getMaterialInstance(subMeshIndex).get(),
        .mesh = mesh,
        .subMeshIndex = static_cast<uint16_t>(subMeshIndex),
        .transform = ((isMeshAnimated) ? &(*skinnedMeshesPremultipliedTransforms.rbegin()) :
          &transform.getTransformationMatrix()),
        .matrixPalette = matrixPalette,
      });

      if (m_isBoundsRenderingEnabled) {
        if (transformComponent.isStatic()) {
          DebugPainter::renderAABB(transformComponent.getBoundingBox());
        }
        else {
          DebugPainter::renderSphere(transformComponent.getBoundingSphere());
        }
      }
    }
  }
}

void MeshRenderingSystem::enableBoundsRendering(bool isEnabled)
{
  m_isBoundsRenderingEnabled = isEnabled;
}

bool MeshRenderingSystem::isBoundsRenderingEnabled() const
{
  return m_isBoundsRenderingEnabled;
}

void MeshRenderingSystem::debugDrawSkeleton(GameObject& gameObject)
{
  auto animationComponent = gameObject.getComponent<SkeletalAnimationComponent>();
  auto& statesMachine = animationComponent->getAnimationStatesMachineRef();

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

    auto& objectTransform = gameObject.getComponent<TransformComponent>()->getTransform();
    auto& inverseSceneTransform =
      gameObject.getComponent<MeshRendererComponent>()->getMeshInstance()->getInverseSceneTransform();

    glm::vec3 boneVectorStart = objectTransform.getTransformationMatrix() * inverseSceneTransform * boneCenter;
    glm::vec3 boneVectorEnd = objectTransform.getTransformationMatrix() * inverseSceneTransform * parentBoneCenter;

    boneVectorStart += objectTransform.getFrontDirection() * m_skeletonsRenderingOffset;
    boneVectorEnd += objectTransform.getFrontDirection() * m_skeletonsRenderingOffset;

    DebugPainter::renderSphere(boneVectorStart, 0.03f);
    DebugPainter::renderSegment(boneVectorStart, boneVectorEnd, {0.0f, 0.8f, 0.0f, 1.0f});

//    DebugPainter::renderVector(gameObject.getComponent<TransformComponent>()->getBoundingSphere().getOrigin(),
//      objectTransform.getFrontDirection(), {0.0f, 0.8f, 0.0f, 1.0f});
  }
}

void MeshRenderingSystem::enableSkeletonsRendering(bool isEnabled)
{
  m_isSkeletonsRenderingEnabled = isEnabled;
}

bool MeshRenderingSystem::isSkeletonsRenderingEnabled() const
{
  return m_isSkeletonsRenderingEnabled;
}
