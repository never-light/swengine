#include "precompiled.h"

#pragma hdrstop

#include "BulletCollisionShapesAdapter.h"

BulletCollisionShapeSphere::BulletCollisionShapeSphere(float radius)
  : CollisionShapeSphere(),
    m_shape(new btSphereShape(radius))
{

}

BulletCollisionShapeSphere::~BulletCollisionShapeSphere()
{
  delete m_shape;
}

float BulletCollisionShapeSphere::getRadius() const
{
  return m_shape->getRadius();
}

btCollisionShape* BulletCollisionShapeSphere::getCollisionShape() const
{
  return m_shape;
}

BulletCollisionShapeBox::BulletCollisionShapeBox(const glm::vec3& halfExtents)
  : CollisionShapeBox(),
    m_shape(new btBoxShape({ halfExtents.x, halfExtents.y, halfExtents.y }))
{
}

glm::vec3 BulletCollisionShapeBox::getHalfExtents() const
{
  btVector3 extents = m_shape->getHalfExtentsWithoutMargin();

  return { extents.x(), extents.y(), extents.z() };
}

BulletCollisionShapeBox::~BulletCollisionShapeBox()
{
  delete m_shape;
}

btCollisionShape* BulletCollisionShapeBox::getCollisionShape() const
{
  return m_shape;
}

BulletCollisionShapeCapsule::BulletCollisionShapeCapsule(float radius, float height)
  : CollisionShapeCapsule(),
  m_shape(new btCapsuleShape(radius, height))
{

}

BulletCollisionShapeCapsule::~BulletCollisionShapeCapsule()
{
  delete m_shape;
}

float BulletCollisionShapeCapsule::getRadius() const
{
  return m_shape->getRadius();
}

float BulletCollisionShapeCapsule::getHeight() const
{
  return m_shape->getRadius();
}

btCollisionShape* BulletCollisionShapeCapsule::getCollisionShape() const
{
  return m_shape;
}
