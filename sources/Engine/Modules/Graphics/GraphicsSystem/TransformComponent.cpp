#include "precompiled.h"

#pragma hdrstop

#include "TransformComponent.h"

TransformComponent::TransformComponent()
  : m_transform(std::make_shared<Transform>())
{

}

Transform& TransformComponent::getTransform()
{
  return *m_transform;
}

const Transform& TransformComponent::getTransform() const
{
  return *m_transform;
}

std::shared_ptr<Transform> TransformComponent::getTransformPtr() const
{
  return m_transform;
}

void TransformComponent::setStaticMode(bool isStatic)
{
  m_isStatic = isStatic;
}

bool TransformComponent::isStatic() const
{
  return m_isStatic;
}

void TransformComponent::updateBounds(const glm::mat4& transformation)
{
  if (m_isStatic) {
    glm::vec3 newMin(std::numeric_limits<float>::max());
    glm::vec3 newMax(std::numeric_limits<float>::lowest());

    for (glm::vec3 corner : m_originalBounds.getCorners()) {
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
    Sphere originalBoundingSphere = m_originalBounds.toSphere();

    glm::vec3 origin = originalBoundingSphere.getOrigin() + glm::vec3(transformation[3]);

    float radiusFactor = glm::max(glm::max(transformation[0][0],
      transformation[1][1]),
      transformation[2][2]);

    float radius = originalBoundingSphere.getRadius() * radiusFactor;

    m_boundingSphere.setOrigin(origin);
    m_boundingSphere.setRadius(radius);
  }
}

const AABB& TransformComponent::getBoundingBox() const
{
  return m_boundingBox;
}

const Sphere& TransformComponent::getBoundingSphere() const
{
  return m_boundingSphere;
}

void TransformComponent::updateBounds(const glm::vec3& origin, const glm::quat& orientation)
{
  if (m_isStatic) {
    updateBounds(glm::translate(glm::identity<glm::mat4>(), origin) * glm::mat4_cast(orientation));
  }
  else {
    glm::vec3 newOrigin = m_originalBounds.toSphere().getOrigin() + origin;
    m_boundingSphere.setOrigin(newOrigin);
  }
}

void TransformComponent::setBounds(const AABB& bounds)
{
  m_originalBounds = bounds;
}

