#include "precompiled.h"

#pragma hdrstop

#include "Modules/Math/MathUtils.h"

#include "BulletRigidBodyComponent.h"

BulletRigidBodyComponent::BulletRigidBodyComponent(float mass,
  std::shared_ptr<CollisionShape> collisionShape,
  std::shared_ptr<Transform> gameTransform)
  : m_collisionShape(convertCollisionShapeToBulletShape(*collisionShape))
{
  btTransform defaultTransform;
  defaultTransform.setIdentity();

  btCollisionShape* btShape = getBtCollisionShape();
  btVector3 localInertia(0, 0, 0);

  if (!MathUtils::isEqual(mass, 0.0f)) {
    btShape->calculateLocalInertia(mass, localInertia);
  }

  m_motionState = new BulletMotionState(defaultTransform, gameTransform);

  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, btShape, localInertia);
  m_rigidBodyInstance = new btRigidBody(rbInfo);
}

BulletRigidBodyComponent::~BulletRigidBodyComponent()
{
  delete m_rigidBodyInstance;
  delete m_motionState;
}

btCollisionShape* BulletRigidBodyComponent::getBtCollisionShape() const
{
  return m_collisionShape;
}

void BulletRigidBodyComponent::setMass(float mass)
{
  btVector3 localInertia(0, 0, 0);

  if (!MathUtils::isEqual(mass, 0.0f)) {
    getBtCollisionShape()->calculateLocalInertia(mass, localInertia);
  }

  m_rigidBodyInstance->setMassProps(mass, localInertia);
}

float BulletRigidBodyComponent::getMass() const
{
  return m_rigidBodyInstance->getMass();
}

void BulletRigidBodyComponent::setTransform(const Transform& transform)
{
  btTransform internalTransform;
  internalTransform.setIdentity();

  const glm::vec3& position = transform.getPosition();
  const glm::quat& orientation = transform.getOrientation();

  internalTransform.setOrigin({position.x, position.y, position.z});
  internalTransform.setRotation({orientation.x, orientation.y, orientation.z, orientation.w});

  m_motionState->setWorldTransform(internalTransform);
  m_rigidBodyInstance->setWorldTransform(internalTransform);
}

btCollisionShape* BulletRigidBodyComponent::convertCollisionShapeToBulletShape(
  const CollisionShape& shape)
{
  if (auto sphereShape = dynamic_cast<const CollisionShapeSphere*>(&shape)) {
    return new btSphereShape(sphereShape->getRadius());
  }
  else if (auto boxShape = dynamic_cast<const CollisionShapeBox*>(&shape)) {
    glm::vec3 boxExtents = boxShape->getHalfExtents();

    return new btBoxShape({boxExtents.x, boxExtents.y, boxExtents.z});
  }
  else if (auto capsuleShape = dynamic_cast<const CollisionShapeCapsule*>(&shape)) {
    return new btCapsuleShape(capsuleShape->getRadius(), capsuleShape->getHeight());
  }
  else {
    SW_ASSERT(false);
  }

  return nullptr;
}

void BulletRigidBodyComponent::setLinearVelocity(const glm::vec3& velocity)
{
  m_rigidBodyInstance->setLinearVelocity(btVector3(velocity.x, velocity.y, velocity.z));
}

glm::vec3 BulletRigidBodyComponent::getLinearVelocity() const
{
  const btVector3& velocity = m_rigidBodyInstance->getLinearVelocity();

  return glm::vec3(velocity.x(), velocity.y(), velocity.z());
}
