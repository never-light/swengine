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

void MeshRenderingSystem::renderForward()
{
}

void MeshRenderingSystem::renderDeferred()
{
  static std::vector<GameObject> visibleObjects;
  visibleObjects.reserve(1024);
  visibleObjects.clear();

  m_graphicsScene->queryVisibleObjects(visibleObjects);

  auto& sharedGraphicsState = *m_graphicsScene->getSharedGraphicsState();
  auto& frameStats = sharedGraphicsState.getFrameStats();

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

    for (size_t subMeshIndex = 0; subMeshIndex < subMeshesCount; subMeshIndex++) {
      Material* material = meshComponent->getMaterialInstance(subMeshIndex).get();

      SW_ASSERT(material != nullptr);

      GLShadersPipeline* shadersPipeline = material->getGpuMaterial().getShadersPipeline().get();

      SW_ASSERT(shadersPipeline != nullptr);

      GLShader* vertexShader = shadersPipeline->getShader(GL_VERTEX_SHADER);

      if (vertexShader->hasParameter("transform.localToWorld")) {
        vertexShader->setParameter("transform.localToWorld", transform.getTransformationMatrix());
      }

      Camera* camera = m_graphicsScene->getActiveCamera().get();

      if (camera != nullptr) {
        if (vertexShader->hasParameter("scene.worldToCamera")) {
          vertexShader->setParameter("scene.worldToCamera", camera->getViewMatrix());
          vertexShader->setParameter("scene.cameraToProjection", camera->getProjectionMatrix());
        }
      }

      if (mesh->isSkinned() && mesh->hasSkeleton() && vertexShader->hasParameter("animation.palette[0]")) {
        if (obj.hasComponent<SkeletalAnimationComponent>() &&
          obj.getComponent<SkeletalAnimationComponent>()->getAnimationStatesMachineRef().isActive()) {
          // Set animation data for shader
          const AnimationStatesMachine& animationStatesMachine =
            obj.getComponent<SkeletalAnimationComponent>()->getAnimationStatesMachineRef();
          const AnimationMatrixPalette& currentMatrixPalette =
            animationStatesMachine.getCurrentMatrixPalette();

          vertexShader->setArrayParameter("animation.palette",
            currentMatrixPalette.bonesTransforms);
        }
        else {
          vertexShader->setArrayParameter("animation.palette",
            std::vector<glm::mat4>(vertexShader->getArraySize("animation.palette"),
              glm::identity<glm::mat4>()));
        }
      }

      frameStats.increasePrimitivesCount(mesh->getSubMeshIndicesCount(subMeshIndex) / 3);

      m_graphicsContext->executeRenderTask({
        &material->getGpuMaterial(),
        mesh->getGeometryStore(),
        mesh->getSubMeshIndicesOffset(subMeshIndex),
        mesh->getSubMeshIndicesCount(subMeshIndex),
        GL_TRIANGLES,
        &sharedGraphicsState.getDeferredFramebuffer()
      });

      if (transformComponent.isStatic()) {
        DebugPainter::renderAABB(transformComponent.getBoundingBox());
      }
      else {
        DebugPainter::renderSphere(transformComponent.getBoundingSphere());
      }
    }
  }
}
