#include "precompiled.h"

#pragma hdrstop

#include "EnvironmentRenderingSystem.h"

#include <utility>
#include "Modules/ECS/ECS.h"

EnvironmentComponent::EnvironmentComponent() = default;

void EnvironmentComponent::setEnvironmentMaterial(ResourceHandle<GLMaterial> material)
{
  m_environmentMaterial = std::move(material);
}

GLMaterial* EnvironmentComponent::getEnvironmentMaterial() const
{
  return m_environmentMaterial.get();
}

EnvironmentComponent::BindingParameters EnvironmentComponent::getBindingParameters() const
{
  return EnvironmentComponent::BindingParameters{
    .materialResourceName = m_environmentMaterial.getResourceId()
  };
}

EnvironmentComponentBinder::EnvironmentComponentBinder(const ComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : m_bindingParameters(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void EnvironmentComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& environmentComponent = *gameObject.addComponent<EnvironmentComponent>().get();

  ResourceHandle<GLMaterial> materialInstance =
    m_resourcesManager->getResource<GLMaterial>(m_bindingParameters.materialResourceName);

  environmentComponent.setEnvironmentMaterial(materialInstance);
}

EnvironmentRenderingSystem::EnvironmentRenderingSystem(
  std::shared_ptr<GLGraphicsContext> graphicsContext,
  std::shared_ptr<GraphicsScene> graphicsScene,
  ResourceHandle<Mesh> environmentMesh)
  : RenderingSystem(std::move(graphicsContext), std::move(graphicsScene)),
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

void EnvironmentRenderingSystem::render()
{
  GameObject environmentObject = getGameWorld()->allWith<EnvironmentComponent>().begin().getGameObject();

  if (!environmentObject.isAlive()) {
    SW_ASSERT(false);
    return;
  }

  GLMaterial* material = environmentObject.getComponent<EnvironmentComponent>()->getEnvironmentMaterial();

  auto& frameStats = m_graphicsScene->getFrameStats();

  frameStats.increaseSubMeshesCount(1);
  frameStats.increasePrimitivesCount(m_environmentMesh->getSubMeshIndicesCount(0) / 3);

  m_graphicsContext->scheduleRenderTask(RenderTask{
    .material = material,
    .mesh = m_environmentMesh.get(),
    .subMeshIndex = 0,
  });
}
