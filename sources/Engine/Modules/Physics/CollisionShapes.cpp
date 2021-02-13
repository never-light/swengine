#include "precompiled.h"

#pragma hdrstop

#include "CollisionShapes.h"

#include <utility>

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

CollisionShapeCompound::CollisionShapeCompound(std::vector<CollisionShapeCompoundChild> children)
  : m_children(std::move(children))
{

}

void CollisionShapeCompound::setChildren(const std::vector<CollisionShapeCompoundChild>& children)
{
  m_children = children;
}

const std::vector<CollisionShapeCompoundChild>& CollisionShapeCompound::getChildren() const
{
  return m_children;
}

CollisionShapeTriangleMesh::CollisionShapeTriangleMesh(std::vector<glm::vec3> vertices)
  : m_vertices(std::move(vertices))
{
  SW_ASSERT(!m_vertices.empty() && m_vertices.size() % 3 == 0);
}

void CollisionShapeTriangleMesh::setVertices(const std::vector<glm::vec3>& vertices)
{
  SW_ASSERT(!vertices.empty() && vertices.size() % 3 == 0);

  m_vertices = vertices;
}

const std::vector<glm::vec3>& CollisionShapeTriangleMesh::getVertices() const
{
  return m_vertices;
}

CollisionShape::CollisionShape(CollisionShapeData  shapeData)
  : m_shapeData(std::move(shapeData))
{

}

void CollisionShape::setShapeData(const CollisionShapeData& shapeData)
{
  m_shapeData = shapeData;
}

const CollisionShapeData& CollisionShape::getShapeData() const
{
  return m_shapeData;
}
