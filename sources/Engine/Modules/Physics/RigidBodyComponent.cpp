#include "precompiled.h"

#pragma hdrstop

#include "RigidBodyComponent.h"

#include <utility>
#include "Modules/ECS/ECS.h"
#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "PhysicsBackendFactory.h"

RigidBodyComponent::RigidBodyComponent(float mass, ResourceHandle<CollisionShape> collisionShape)
  : m_collisionShape(collisionShape),
  m_backend(PhysicsBackendFactory::createRigidBodyComponent(mass, collisionShape))
{

}

void RigidBodyComponent::setMass(float mass)
{
  m_backend->setMass(mass);
}

float RigidBodyComponent::getMass() const
{
  return m_backend->getMass();
}

void RigidBodyComponent::setTransform(const Transform& transform)
{
  m_backend->setTransform(transform);
}

void RigidBodyComponent::setLinearVelocity(const glm::vec3& velocity)
{
  m_backend->setLinearVelocity(velocity);
}

glm::vec3 RigidBodyComponent::getLinearVelocity() const
{
  return m_backend->getLinearVelocity();
}

void RigidBodyComponent::setCollisionCallback(CollisionCallback callback)
{
  m_collisionCallback = std::move(callback);
}

CollisionCallback RigidBodyComponent::getCollisionCallback() const
{
  return m_collisionCallback;
}

const RigidBodyComponentBackend& RigidBodyComponent::getBackend() const
{
  return *m_backend;
}

void RigidBodyComponent::resetBackend()
{
  m_backend = nullptr;
}

void RigidBodyComponent::setAngularFactor(const glm::vec3& factor)
{
  m_backend->setAngularFactor(factor);
}

[[nodiscard]] glm::vec3 RigidBodyComponent::getAngularFactor() const
{
  return m_backend->getAngularFactor();
}

void RigidBodyComponent::setLinearFactor(const glm::vec3& factor)
{
  m_backend->setLinearFactor(factor);
}

RigidBodyComponentBackend& RigidBodyComponent::getBackend()
{
  return *m_backend;
}

[[nodiscard]] glm::vec3 RigidBodyComponent::getLinearFactor() const
{
  return m_backend->getLinearFactor();
}

RigidBodyComponent::BindingParameters RigidBodyComponent::getBindingParameters() const
{
  return RigidBodyComponent::BindingParameters{
    .collisionModelResourceName = m_collisionShape.getResourceId(),
    .mass = getMass(),
  };
}


RigidBodyComponentBinder::RigidBodyComponentBinder(const ComponentBindingParameters& componentParameters,
  std::shared_ptr<ResourcesManager> resourcesManager)
  : m_bindingParameters(componentParameters),
    m_resourcesManager(std::move(resourcesManager))
{

}

void RigidBodyComponentBinder::bindToObject(GameObject& gameObject)
{
  const std::string& collisionModelName = m_bindingParameters.collisionModelResourceName;

  ResourceHandle<CollisionShape> collisionShape;

  auto transformComponent = gameObject.getComponent<TransformComponent>();
//  Transform& objectTransform = transformComponent->getTransform();

  AABB objectBounds = transformComponent->getOriginalBounds();

//  glm::mat4 objectBoundsTransform =
//      MathUtils::getScaleMatrix(objectTransform.getScale());
//
//  objectBounds.applyTransform(objectBoundsTransform);

  if (collisionModelName == "visual_aabb") {
    CollisionShapeData colliderAABB = CollisionShapeCompound({
      CollisionShapeCompoundChild{CollisionShapeBox(objectBounds.getSize() * 0.5f),
        objectBounds.getOrigin()}});

    collisionShape = m_resourcesManager->createResourceInPlace<CollisionShape>(colliderAABB);
  }
  else if (collisionModelName == "visual_sphere") {
    CollisionShapeData colliderSphere = CollisionShapeCompound({
      CollisionShapeCompoundChild{CollisionShapeSphere(objectBounds.toSphere().getRadius()),
        objectBounds.toSphere().getOrigin()}});

    collisionShape = m_resourcesManager->createResourceInPlace<CollisionShape>(colliderSphere);
  }
  else {
    collisionShape = m_resourcesManager->getResource<CollisionShape>(collisionModelName);
  }

  RigidBodyComponent* rigidBodyComponent;

  if (transformComponent->isStatic()) {
    SW_ASSERT(MathUtils::isZero(m_bindingParameters.mass));

    // Set zero mass to mark rigid body as static body
    rigidBodyComponent = gameObject.addComponent<RigidBodyComponent>(0.0f, collisionShape).get();
  }
  else {
    rigidBodyComponent = gameObject.addComponent<RigidBodyComponent>(m_bindingParameters.mass, collisionShape).get();
  }

  LOCAL_VALUE_UNUSED(rigidBodyComponent);
}
