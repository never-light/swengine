#include "precompiled.h"

#pragma hdrstop

#include "TransformComponent.h"

#include <utility>
#include "Modules/ECS/ECS.h"

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
  // TODO: it is possible to optimize update operation for spheres (dynamic meshes), so consider to
  //  perform such optimization.
  updateBounds(MathUtils::buildTRSMatrix(origin, orientation, getTransform().getScale()));
}

void TransformComponent::setBounds(const AABB& bounds)
{
  m_originalBounds = bounds;
}

const AABB& TransformComponent::getOriginalBounds() const
{
  return m_originalBounds;
}

TransformComponent::BindingParameters TransformComponent::getBindingParameters() const
{
  return TransformComponent::BindingParameters{
    .position = m_transform->getPosition(),
    .scale = m_transform->getScale(),
    .frontDirection = glm::eulerAngles(m_transform->getOrientation()),
    .isStatic = m_isStatic,
    .levelId = m_levelId
  };
}

void TransformComponent::setLevelId(const std::string& levelId)
{
  m_levelId = levelId;
}

const std::string& TransformComponent::getLevelId() const
{
  return m_levelId;
}

void TransformComponent::setOnlineMode(bool isOnline)
{
  m_isOnline = isOnline;
}

bool TransformComponent::isOnline() const
{
  return m_isOnline;
}

TransformComponentBinder::TransformComponentBinder(ComponentBindingParameters  componentParameters)
  : m_bindingParameters(std::move(componentParameters))
{

}

void TransformComponentBinder::bindToObject(GameObject& gameObject)
{
  auto& transformComponent = *gameObject.addComponent<TransformComponent>().get();

  transformComponent.setStaticMode(m_bindingParameters.isStatic);
  transformComponent.getTransform().setPosition(m_bindingParameters.position);
  transformComponent.getTransform()
    .setOrientation(glm::quat(glm::vec3(
      glm::radians(m_bindingParameters.frontDirection.x), glm::radians(m_bindingParameters.frontDirection.y),
      glm::radians(m_bindingParameters.frontDirection.z))));
  transformComponent.getTransform().setScale(m_bindingParameters.scale);
  transformComponent.setLevelId(m_bindingParameters.levelId);
  transformComponent.setOnlineMode(m_bindingParameters.isOnline);
}
