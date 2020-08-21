#include "precompiled.h"

#pragma hdrstop

#include "CollisionShapes.h"

void CollisionShapeSphere::setRadius(float radius)
{
  m_radius = radius;
}

float CollisionShapeSphere::getRadius() const
{
  return m_radius;
}

CollisionShapeSphere::CollisionShapeSphere(float radius)
  : m_radius(radius)
{

}

void CollisionShapeBox::setHalfExtents(const glm::vec3& halfExtents)
{
  m_halfExtents = halfExtents;
}

glm::vec3 CollisionShapeBox::getHalfExtents() const
{
  return m_halfExtents;
}

CollisionShapeBox::CollisionShapeBox(const glm::vec3& halfExtents)
  : m_halfExtents(halfExtents)
{

}

void CollisionShapeCapsule::setRadius(float radius)
{
  m_radius = radius;
}

float CollisionShapeCapsule::getRadius() const
{
  return m_radius;
}

void CollisionShapeCapsule::setHeight(float height)
{
  m_height = height;
}

float CollisionShapeCapsule::getHeight() const
{
  return m_height;
}

CollisionShapeCapsule::CollisionShapeCapsule(float radius, float height)
  : m_radius(radius),
  m_height(height)
{

}
