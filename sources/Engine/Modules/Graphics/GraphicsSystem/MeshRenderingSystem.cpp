#include "precompiled.h"

#pragma hdrstop

#include "MeshRenderingSystem.h"

#include <utility>

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
        .transform = ((isMeshAnimated) ? &(*skinnedMeshesPremultipliedTransforms.rbegin()) : &transform.getTransformationMatrix()),
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
