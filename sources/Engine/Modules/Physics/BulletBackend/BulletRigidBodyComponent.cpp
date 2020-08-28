#include "precompiled.h"

#pragma hdrstop

#include "Modules/Math/MathUtils.h"
#include "BulletRigidBodyComponent.h"
#include "BulletHelpers.h"

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
  else if (auto triangleMeshShape = dynamic_cast<const CollisionShapeTriangleMesh*>(&shape)) {
    // TODO: optimize it, use indices instead of duplicated vertices

    auto* btTriangleMeshShape = new btTriangleMesh();

    const auto& vertices = triangleMeshShape->getVertices();

    for (size_t vertexIndex = 0; vertexIndex < vertices.size(); vertexIndex += 3) {
      btTriangleMeshShape->addTriangle(glmVec3ToBt(vertices[vertexIndex]),
        glmVec3ToBt(vertices[vertexIndex + 1]),
        glmVec3ToBt(vertices[vertexIndex + 2]));
    }

    return new btBvhTriangleMeshShape(btTriangleMeshShape, true);
  }
  else if (auto compoundShape = dynamic_cast<const CollisionShapeCompound*>(&shape)) {
    auto* btCompound = new btCompoundShape();

    for (const auto& childShape : compoundShape->getChildren()) {
      btTransform transform;
      transform.setOrigin(glmVec3ToBt(childShape.origin));

      btCompound->addChildShape(transform, convertCollisionShapeToBulletShape(*childShape.shape));
    }

    return btCompound;
  }
  else {
    SW_ASSERT(false);
  }

  return nullptr;
}

void BulletRigidBodyComponent::setLinearVelocity(const glm::vec3& velocity)
{
  m_rigidBodyInstance->setLinearVelocity(glmVec3ToBt(velocity));
}

glm::vec3 BulletRigidBodyComponent::getLinearVelocity() const
{
  const btVector3& velocity = m_rigidBodyInstance->getLinearVelocity();

  return glm::vec3(velocity.x(), velocity.y(), velocity.z());
}

void BulletRigidBodyComponent::setAngularFactor(const glm::vec3& factor)
{
  m_rigidBodyInstance->setAngularFactor(glmVec3ToBt(factor));
}

glm::vec3 BulletRigidBodyComponent::getAngularFactor() const
{
  return btVec3ToGlm(m_rigidBodyInstance->getAngularFactor());
}
