#include "precompiled.h"
#pragma hdrstop

#include "MeshRendererComponent.h"

MeshRendererComponent::MeshRendererComponent()
{

}

void MeshRendererComponent::setMeshInstance(std::shared_ptr<Mesh> instance)
{
    m_meshInstance = instance;

    m_materialsInstances.resize(m_meshInstance->getSubMeshesCount());
    std::fill(m_materialsInstances.begin(), m_materialsInstances.end(), nullptr);


    m_aabb = m_meshInstance->getAABB();
}

std::shared_ptr<Mesh> MeshRendererComponent::getMeshInstance() const
{
    return m_meshInstance;
}

void MeshRendererComponent::setMaterialsInstances(const std::vector<std::shared_ptr<Material>>& instances)
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

const AABB& MeshRendererComponent::getAABB() const
{
    return m_aabb;
}

void MeshRendererComponent::updateBounds(Transform& transform)
{
    const glm::mat4& transformMatrix = transform.getTransformationMatrix();

    glm::vec3 newMin(std::numeric_limits<float>::max());
    glm::vec3 newMax(std::numeric_limits<float>::min());

    for (glm::vec3 corner : m_aabb.getCorners()) {
        glm::vec4 newCorner = transformMatrix * glm::vec4(corner, 1.0f);

        newMin.x = std::fminf(newMin.x, newCorner.x);
        newMin.y = std::fminf(newMin.y, newCorner.y);
        newMin.z = std::fminf(newMin.z, newCorner.z);

        newMax.x = std::fmaxf(newMax.x, newCorner.x);
        newMax.y = std::fmaxf(newMax.y, newCorner.y);
        newMax.z = std::fmaxf(newMax.z, newCorner.z);
    }

    m_aabb.setMin(newMin);
    m_aabb.setMax(newMax);
}
