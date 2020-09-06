#include "precompiled.h"

#pragma hdrstop

#include "MeshRendererComponent.h"

#include <utility>

MeshRendererComponent::MeshRendererComponent() = default;

void MeshRendererComponent::setMeshInstance(std::shared_ptr<Mesh> instance)
{
  m_meshInstance = std::move(instance);

  m_materialsInstances.resize(m_meshInstance->getSubMeshesCount());
  std::fill(m_materialsInstances.begin(), m_materialsInstances.end(), nullptr);

  m_boundingBox = m_meshInstance->getAABB();
  m_boundingSphere = m_boundingBox.toSphere();
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

  m_materialsInstances[subMeshIndex] = std::move(instance);
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

void MeshRendererComponent::updateBounds(const glm::mat4& transformation)
{
  if (m_renderingAttributes.isStatic) {
    glm::vec3 newMin(std::numeric_limits<float>::max());
    glm::vec3 newMax(std::numeric_limits<float>::lowest());

    for (glm::vec3 corner : m_meshInstance->getAABB().getCorners()) {
      glm::vec4 newCorner = transformation * glm::vec4(corner, 1.0f);

      newMin.x = std::fminf(newMin.x, newCorner.x);
      newMin.y = std::fminf(newMin.y, newCorner.y);
      newMin.z = std::fminf(newMin.z, newCorner.z);

      newMax.x = std::fmaxf(newMax.x, newCorner.x);
      newMax.y = std::fmaxf(newMax.y, newCorner.y);
      newMax.z = std::fmaxf(newMax.z, newCorner.z);
    }

    m_boundingBox.setMin(newMin);
    m_boundingBox.setMax(newMax);
  }
  else {
    Sphere originalBoundingSphere = m_meshInstance->getAABB().toSphere();

    glm::vec3 origin = originalBoundingSphere.getOrigin() + glm::vec3(transformation[3]);

    float radiusFactor = glm::max(glm::max(transformation[0][0],
      transformation[1][1]),
      transformation[2][2]);

    float radius = originalBoundingSphere.getRadius() * radiusFactor;

    m_boundingSphere.setOrigin(origin);
    m_boundingSphere.setRadius(radius);
  }
}

const MeshRenderingAttributes& MeshRendererComponent::getAttributes() const
{
  return m_renderingAttributes;
}

MeshRenderingAttributes& MeshRendererComponent::getAttributes()
{
  return m_renderingAttributes;
}

const AABB& MeshRendererComponent::getBoundingBox() const
{
  return m_boundingBox;
}

const Sphere& MeshRendererComponent::getBoundingSphere() const
{
  return m_boundingSphere;
}

void MeshRendererComponent::updateBounds(const glm::vec3& origin, const glm::quat& orientation)
{
  if (m_renderingAttributes.isStatic) {
    updateBounds(glm::translate(glm::identity<glm::mat4>(), origin) * glm::mat4_cast(orientation));
  }
  else {
    glm::vec3 newOrigin = m_meshInstance->getAABB().toSphere().getOrigin() + origin;
    m_boundingSphere.setOrigin(newOrigin);
  }
}

