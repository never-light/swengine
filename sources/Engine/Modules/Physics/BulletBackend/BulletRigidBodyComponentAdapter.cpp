#include "precompiled.h"

#pragma hdrstop

#include "Modules/Math/MathUtils.h"

#include "BulletRigidBodyComponentAdapter.h"

BulletRigidBodyComponentAdapter::BulletRigidBodyComponentAdapter(float mass,
  std::shared_ptr<CollisionShape> collisionShape)
  : m_collisionShape(convertCollisionShapeToBulletShape(*collisionShape))
{
  btTransform defaultTransform;
  defaultTransform.setIdentity();

  btCollisionShape* btShape = getBtCollisionShape();
  auto t = btShape->getShapeType();
  LOCAL_VALUE_UNUSED(t);

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

btCollisionShape* BulletRigidBodyComponentAdapter::getBtCollisionShape() const
{
  return m_collisionShape;
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

  internalTransform.setOrigin({position.x, position.y, position.z});
  internalTransform.setRotation({orientation.x, orientation.y, orientation.z, orientation.w});

  m_motionState->setWorldTransform(internalTransform);
}

btCollisionShape* BulletRigidBodyComponentAdapter::convertCollisionShapeToBulletShape(
  const CollisionShape& shape)
{
  if (auto sphereShape = dynamic_cast<const CollisionShapeSphere*>(&shape)) {
    return new btSphereShape(sphereShape->getRadius());
  }
  else if (auto boxShape = dynamic_cast<const CollisionShapeBox*>(&shape)) {
    glm::vec3 boxExtents = boxShape->getHalfExtents();

    return new btBoxShape({ boxExtents.x, boxExtents.y, boxExtents.z });
  }
  else if (auto capsuleShape = dynamic_cast<const CollisionShapeCapsule*>(&shape)) {
    return new btCapsuleShape(capsuleShape->getRadius(), capsuleShape->getHeight());
  }
  else {
    SW_ASSERT(false);
  }

  return nullptr;
}

void BulletRigidBodyComponentAdapter::requestTransform(Transform& transform) const
{
  btTransform bodyTransform;
  m_rigidBodyInstance->getMotionState()->getWorldTransform(bodyTransform);

  const btVector3& origin = bodyTransform.getOrigin();
  const btQuaternion& orientation = bodyTransform.getRotation();

  transform.setPosition(origin.x(), origin.y(), origin.z());
  transform.setOrientation({ orientation.w(), orientation.x(), orientation.y(), orientation.z() });
}
