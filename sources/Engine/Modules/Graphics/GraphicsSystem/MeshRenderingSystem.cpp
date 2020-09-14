#include "precompiled.h"

#pragma hdrstop

#include "MeshRenderingSystem.h"

#include <utility>

#include "Modules/ECS/ECS.h"
#include "Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationComponent.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"

MeshRenderingSystem::MeshRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<SharedGraphicsState> sharedGraphicsState)
  : RenderingSystem(std::move(graphicsContext), std::move(sharedGraphicsState))
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
  for (GameObject obj : getGameWorld()->allWith<MeshRendererComponent>()) {
    auto meshComponent = obj.getComponent<MeshRendererComponent>();

    if (meshComponent->isCulled()) {
      continue;
    }

    Mesh* mesh = meshComponent->getMeshInstance().get();

    SW_ASSERT(mesh != nullptr);

    Transform& transform = obj.getComponent<TransformComponent>()->getTransform();

    const size_t subMeshesCount = mesh->getSubMeshesCount();

    SW_ASSERT(subMeshesCount != 0);

    m_sharedGraphicsState->getFrameStats().increaseSubMeshesCount(subMeshesCount);

    for (size_t subMeshIndex = 0; subMeshIndex < subMeshesCount; subMeshIndex++) {
      Material* material = meshComponent->getMaterialInstance(subMeshIndex).get();

      SW_ASSERT(material != nullptr);

      GLShadersPipeline* shadersPipeline = material->getGpuMaterial().getShadersPipeline().get();

      SW_ASSERT(shadersPipeline != nullptr);

      GLShader* vertexShader = shadersPipeline->getShader(GL_VERTEX_SHADER);

      if (vertexShader->hasParameter("transform.localToWorld")) {
        vertexShader->setParameter("transform.localToWorld", transform.getTransformationMatrix());
      }

      Camera* camera = m_sharedGraphicsState->getActiveCamera().get();

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

      m_sharedGraphicsState->getFrameStats().increasePrimitivesCount(mesh->getSubMeshIndicesCount(subMeshIndex) / 3);

      m_graphicsContext->executeRenderTask({
        &material->getGpuMaterial(),
        mesh->getGeometryStore(),
        mesh->getSubMeshIndicesOffset(subMeshIndex),
        mesh->getSubMeshIndicesCount(subMeshIndex),
        GL_TRIANGLES,
        &m_sharedGraphicsState->getDeferredFramebuffer()
      });
    }
  }
}
