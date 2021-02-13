#include "precompiled.h"

#pragma hdrstop

#include "KinematicCharacterComponent.h"

#include <utility>
#include "Modules/ECS/ECS.h"
#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "PhysicsBackendFactory.h"

KinematicCharacterComponent::KinematicCharacterComponent(ResourceHandle<CollisionShape> collisionShape)
  : m_collisionShape(collisionShape),
    m_backend(PhysicsBackendFactory::createKinematicCharacterComponent(std::move(collisionShape)))
{

}

void KinematicCharacterComponent::setTransform(const Transform& transform)
{
  m_backend->setTransform(transform);
}

void KinematicCharacterComponent::setPositionIncrement(const glm::vec3& increment)
{
  m_backend->setPositionIncrement(increment);
}

void KinematicCharacterComponent::jump(const glm::vec3& jumpVector)
{
  m_backend->jump(jumpVector);
}

bool KinematicCharacterComponent::isOnGround() const
{
  return m_backend->isOnGround();
}

void KinematicCharacterComponent::setCollisionCallback(CollisionCallback callback)
{
  m_collisionCallback = std::move(callback);
}

CollisionCallback KinematicCharacterComponent::getCollisionCallback() const
{
  return m_collisionCallback;
}

const KinematicCharacterComponentBackend& KinematicCharacterComponent::getBackend() const
{
  return *m_backend;
}

KinematicCharacterComponentBackend& KinematicCharacterComponent::getBackend()
{
  return *m_backend;
}

void KinematicCharacterComponent::resetBackend()
{
  m_backend = nullptr;
}

void KinematicCharacterComponent::setOriginOffset(const glm::vec3& offset)
{
  m_backend->setOriginOffset(offset);
}

glm::vec3 KinematicCharacterComponent::getOriginOffset() const
{
  return m_backend->getOriginOffset();
}

ResourceHandle<CollisionShape> KinematicCharacterComponent::getCollisionShape() const
{
  return m_collisionShape;
}

void KinematicCharacterComponent::enableSimulation(bool enable)
{
  m_backend->enableSimulation(enable);
}

bool KinematicCharacterComponent::isSimulationEnabled() const
{
  return m_backend->isSimulationEnabled();
}

KinematicCharacterComponent::BindingParameters KinematicCharacterComponent::getBindingParameters() const
{
  return KinematicCharacterComponent::BindingParameters{
    .originOffset = getOriginOffset(),
    .capsuleHeight = std::get<CollisionShapeCapsule>(getCollisionShape()->getShapeData()).getHeight(),
    .capsuleRadius = std::get<CollisionShapeCapsule>(getCollisionShape()->getShapeData()).getRadius(),
  };
}

KinematicCharacterComponentBinder::KinematicCharacterComponentBinder(
  const ComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : m_bindingParameters(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void KinematicCharacterComponentBinder::bindToObject(GameObject& gameObject)
{
  ResourceHandle<CollisionShape> collisionShape = m_resourcesManager->createResourceInPlace<CollisionShape>(
    CollisionShapeCapsule(m_bindingParameters.capsuleRadius, m_bindingParameters.capsuleHeight));

  auto& kinematicCharacterComponent =
    *gameObject.addComponent<KinematicCharacterComponent>(collisionShape).get();

  kinematicCharacterComponent.setOriginOffset(m_bindingParameters.originOffset);

  auto& objectTransform = *gameObject.getComponent<TransformComponent>().get();
  kinematicCharacterComponent.setTransform(objectTransform.getTransform());
}
