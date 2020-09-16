#include "precompiled.h"

#pragma hdrstop

#include "EnvironmentRenderingSystem.h"

#include <utility>
#include "Modules/ECS/ECS.h"

EnvironmentComponent::EnvironmentComponent() = default;

void EnvironmentComponent::setEnvironmentMaterial(std::shared_ptr<Material> material)
{
  m_environmentMaterial = std::move(material);
}

Material* EnvironmentComponent::getEnvironmentMaterial() const
{
  return m_environmentMaterial.get();
}

EnvironmentRenderingSystem::EnvironmentRenderingSystem(std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<SharedGraphicsState> sharedGraphicsState,
  std::shared_ptr<Mesh> environmentMesh)
  : RenderingSystem(std::move(graphicsContext), std::move(sharedGraphicsState)),
    m_environmentMesh(std::move(environmentMesh))
{

}

EnvironmentRenderingSystem::~EnvironmentRenderingSystem() = default;

void EnvironmentRenderingSystem::configure()
{
}

void EnvironmentRenderingSystem::unconfigure()
{
}

void EnvironmentRenderingSystem::update(float delta)
{
  ARG_UNUSED(delta);
}

void EnvironmentRenderingSystem::renderForward()
{
  GameObject environmentObject = getGameWorld()->allWith<EnvironmentComponent>().begin().getGameObject();

  if (!environmentObject.isAlive()) {
    SW_ASSERT(false);
    return;
  }

  Material* material = environmentObject.getComponent<EnvironmentComponent>()->getEnvironmentMaterial();

  Camera* camera = m_sharedGraphicsState->getActiveCamera().get();

  if (camera != nullptr) {
    GLShader* vertexShader = material->getGpuMaterial().getShadersPipeline()->getShader(GL_VERTEX_SHADER);

    if (vertexShader->hasParameter("scene.worldToCamera")) {
      glm::mat4 untranslatedViewMatrix = camera->getViewMatrix();
      untranslatedViewMatrix[3] = glm::vec4(0, 0, 0, 1);

      vertexShader->setParameter("scene.worldToCamera", untranslatedViewMatrix);
      vertexShader->setParameter("scene.cameraToProjection", camera->getProjectionMatrix());
    }
  }

  m_sharedGraphicsState->getFrameStats().increaseSubMeshesCount(1);
  m_sharedGraphicsState->getFrameStats().increasePrimitivesCount(m_environmentMesh->getSubMeshIndicesCount(0) / 3);

  m_graphicsContext->executeRenderTask(RenderTask{&material->getGpuMaterial(),
    m_environmentMesh->getGeometryStore(), 0, m_environmentMesh->getSubMeshIndicesCount(0),
    GL_TRIANGLES, &m_sharedGraphicsState->getForwardFramebuffer()
  });

}
