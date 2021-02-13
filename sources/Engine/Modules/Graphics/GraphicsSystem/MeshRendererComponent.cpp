#include "precompiled.h"

#pragma hdrstop

#include "MeshRendererComponent.h"

#include <utility>

#include "Modules/ECS/ECS.h"
#include "TransformComponent.h"

MeshRendererComponent::MeshRendererComponent() = default;

void MeshRendererComponent::setMeshInstance(ResourceHandle<Mesh> instance)
{
  m_meshInstance = instance;

  m_materialsInstances.resize(m_meshInstance->getSubMeshesCount());
}

ResourceHandle<Mesh> MeshRendererComponent::getMeshInstance() const
{
  return m_meshInstance;
}

void MeshRendererComponent::setMaterialsInstances(const std::vector<ResourceHandle<GLMaterial>>& instances)
{
  SW_ASSERT(instances.size() == m_meshInstance->getSubMeshesCount());

  m_materialsInstances = instances;
}

void MeshRendererComponent::setMaterialInstance(size_t subMeshIndex, ResourceHandle<GLMaterial> instance)
{
  SW_ASSERT(subMeshIndex < m_materialsInstances.size());

  m_materialsInstances[subMeshIndex] = std::move(instance);
}

ResourceHandle<GLMaterial> MeshRendererComponent::getMaterialInstance(size_t subMeshIndex) const
{
  SW_ASSERT(subMeshIndex < m_materialsInstances.size());

  return m_materialsInstances[subMeshIndex];
}

const MeshRenderingAttributes& MeshRendererComponent::getAttributes() const
{
  return m_renderingAttributes;
}

MeshRenderingAttributes& MeshRendererComponent::getAttributes()
{
  return m_renderingAttributes;
}

MeshRendererComponent::BindingParameters MeshRendererComponent::getBindingParameters() const
{
  MeshRendererComponent::BindingParameters bindingParameters{
    .meshResourceName = m_meshInstance.getResourceId(),
    .skeletonResourceName = m_meshInstance->hasSkeleton() ? m_meshInstance->getSkeleton().getResourceId() : "",
    .materials = {},
  };

  for (size_t subMeshIndex = 0; const auto& materialInstance : m_materialsInstances) {
    bindingParameters.materials.emplace_back(materialInstance.getResourceId(), subMeshIndex);
    subMeshIndex++;
  }

  return bindingParameters;
}

MeshRendererComponentBinder::MeshRendererComponentBinder(const ComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : m_bindingParameters(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void MeshRendererComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& transformComponent = *gameObject.getComponent<TransformComponent>().get();
  auto& meshRendererComponent = *gameObject.addComponent<MeshRendererComponent>().get();

  ResourceHandle<Mesh> meshInstance =
    m_resourcesManager->getResource<Mesh>(m_bindingParameters.meshResourceName);

  const std::string& skeletonResourceName = m_bindingParameters.skeletonResourceName;

  if (!skeletonResourceName.empty()) {
    meshInstance->setSkeleton(m_resourcesManager->getResource<Skeleton>(skeletonResourceName));
  }

  meshRendererComponent.setMeshInstance(meshInstance);

  transformComponent.setBounds(meshInstance->getAABB());
  transformComponent.updateBounds(transformComponent.getTransform().getTransformationMatrix());

  for (auto&[materialName, subMeshIndex] : m_bindingParameters.materials) {
    ResourceHandle<GLMaterial> materialInstance =
      m_resourcesManager->getResource<GLMaterial>(materialName);

    meshRendererComponent.setMaterialInstance(subMeshIndex, materialInstance);
  }
}
