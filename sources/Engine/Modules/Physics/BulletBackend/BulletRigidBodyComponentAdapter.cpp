#include "precompiled.h"

#pragma hdrstop

#include "Modules/Math/MathUtils.h"

#include "BulletRigidBodyComponentAdapter.h"
#include "BulletCollisionShapesAdapter.h"

BulletRigidBodyComponentAdapter::BulletRigidBodyComponentAdapter(float mass,
  std::shared_ptr<CollisionShape> collisionShape)
    : m_collisionShape(collisionShape)
{
  btTransform defaultTransform;
  defaultTransform.setIdentity();

  btCollisionShape* btShape = getBtCollisionShape();
  btVector3 localInertia(0, 0, 0);

  if (!MathUtils::isEqual(mass, 0.0f)) {
    btShape->calculateLocalInertia(mass, localInertia);
  }

  m_motionState = new btDefaultMotionState(defaultTransform);

  btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_motionState, btShape, localInertia);
  m_rigidBodyInstance = new btRigidBody(rbInfo);
}

BulletRigidBodyComponentAdapter::~BulletRigidBodyComponentAdapter()
{
  delete m_rigidBodyInstance;
  delete m_motionState;
}

const CollisionShape& BulletRigidBodyComponentAdapter::getCollisionShape() const
{
  return *m_collisionShape;
}

btCollisionShape* BulletRigidBodyComponentAdapter::getBtCollisionShape() const
{
  return std::reinterpret_pointer_cast<BulletCollisionShape>(m_collisionShape)->getCollisionShape();
}

void BulletRigidBodyComponentAdapter::setMass(float mass)
{
  btVector3 localInertia(0, 0, 0);

  if (!MathUtils::isEqual(mass, 0.0f)) {
    getBtCollisionShape()->calculateLocalInertia(mass, localInertia);
  }

  m_rigidBodyInstance->setMassProps(mass, localInertia);
}

float BulletRigidBodyComponentAdapter::getMass() const
{
  return m_rigidBodyInstance->getMass();
}

void BulletRigidBodyComponentAdapter::setTransform(const Transform& transform)
{
  btTransform internalTransform;
  internalTransform.setIdentity();

  const glm::vec3& position = transform.getPosition();
  const glm::quat& orientation = transform.getOrientation();

  internalTransform.setOrigin({ position.x, position.y, position.z });
  internalTransform.setRotation({ orientation.x, orientation.y, orientation.z, orientation.w });

  m_motionState->setWorldTransform(internalTransform);
}
