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
    m_boundingBox = m_originalBounds;
    m_boundingBox.applyTransform(transformation);
  }
  else {
    m_boundingSphere = m_originalBounds.toSphere();
    m_boundingSphere.applyTransform(transformation);
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

const AABB& TransformComponent::getOriginalBounds() const
{
  return m_originalBounds;
}

