#include "precompiled.h"

#pragma hdrstop

#include "EnvironmentRenderingSystem.h"

#include <utility>
#include "Modules/ECS/ECS.h"

EnvironmentComponent::EnvironmentComponent() = default;

void EnvironmentComponent::setEnvironmentMaterial(ResourceHandle<Material> material)
{
  m_environmentMaterial = material;
}

Material* EnvironmentComponent::getEnvironmentMaterial() const
{
  return m_environmentMaterial.get();
}

EnvironmentRenderingSystem::EnvironmentRenderingSystem(
  std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<GraphicsScene> graphicsScene,
  ResourceHandle<Mesh> environmentMesh)
  : RenderingSystem(std::move(graphicsContext), std::move(graphicsScene)),
    m_environmentMesh(environmentMesh)
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

  Camera* camera = m_graphicsScene->getActiveCamera().get();

  if (camera != nullptr) {
    GLShader* vertexShader = material->getGpuMaterial().getShadersPipeline()->getShader(ShaderType::Vertex);

    if (vertexShader->hasParameter("scene.worldToCamera")) {
      glm::mat4 untranslatedViewMatrix = camera->getViewMatrix();
      untranslatedViewMatrix[3] = glm::vec4(0, 0, 0, 1);

      vertexShader->setParameter("scene.worldToCamera", untranslatedViewMatrix);
      vertexShader->setParameter("scene.cameraToProjection", camera->getProjectionMatrix());
    }
  }

  auto& shaderGraphicsState = *m_graphicsScene->getSharedGraphicsState();

  shaderGraphicsState.getFrameStats().increaseSubMeshesCount(1);
  shaderGraphicsState.getFrameStats().increasePrimitivesCount(m_environmentMesh->getSubMeshIndicesCount(0) / 3);

  m_graphicsContext->executeRenderTask(RenderTask{&material->getGpuMaterial(),
    m_environmentMesh->getGeometryStore(), 0, m_environmentMesh->getSubMeshIndicesCount(0),
    GL_TRIANGLES, &shaderGraphicsState.getForwardFramebuffer()
  });

}
