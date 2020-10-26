#include "precompiled.h"

#pragma hdrstop

#include "MeshRendererComponent.h"

#include <utility>

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

void MeshRendererComponent::setMaterialsInstances(const std::vector<ResourceHandle<Material>>& instances)
{
  SW_ASSERT(instances.size() == m_meshInstance->getSubMeshesCount());

  m_materialsInstances = instances;
}

void MeshRendererComponent::setMaterialInstance(size_t subMeshIndex, ResourceHandle<Material> instance)
{
  SW_ASSERT(subMeshIndex < m_materialsInstances.size());

  m_materialsInstances[subMeshIndex] = instance;
}

ResourceHandle<Material> MeshRendererComponent::getMaterialInstance(size_t subMeshIndex) const
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

