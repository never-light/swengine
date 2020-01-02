#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent()
{

}

void MeshRendererComponent::setMeshInstance(std::shared_ptr<Mesh> instance)
{
    m_meshInstance = instance;

    m_materialsInstances.resize(m_meshInstance->getSubMeshesCount());
    std::fill(m_materialsInstances.begin(), m_materialsInstances.end(), nullptr);
}

std::shared_ptr<Mesh> MeshRendererComponent::getMeshInstance() const
{
    return m_meshInstance;
}

void MeshRendererComponent::setMaterialsInstances(const std::vector<std::shared_ptr<Material> >& instances)
{
    SW_ASSERT(instances.size() == m_meshInstance->getSubMeshesCount());

    m_materialsInstances = instances;
}

void MeshRendererComponent::setMaterialInstance(size_t subMeshIndex, std::shared_ptr<Material> instance)
{
    SW_ASSERT(subMeshIndex < m_materialsInstances.size());

    m_materialsInstances[subMeshIndex] = instance;
}

std::shared_ptr<Material> MeshRendererComponent::getMaterialInstance(size_t subMeshIndex) const
{
    SW_ASSERT(subMeshIndex < m_materialsInstances.size());

    return m_materialsInstances[subMeshIndex];
}

bool MeshRendererComponent::isCulled() const
{
    return m_isCulled;
}

void MeshRendererComponent::cull(bool culled)
{
    m_isCulled = culled;
}
