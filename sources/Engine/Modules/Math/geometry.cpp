#include "precompiled.h"

#pragma hdrstop

#include "geometry.h"

#include <glm/geometric.hpp>

Plane::Plane()
{

}

Plane::Plane(const glm::vec3& normal, float distance)
  : m_normal(normal), m_distance(distance)
{

}

void Plane::setNormal(const glm::vec3& normal)
{
  m_normal = normal;
}

glm::vec3 Plane::getNormal() const
{
  return m_normal;
}

void Plane::setDistance(float distance)
{
  m_distance = distance;
}

float Plane::getDistance() const
{
  return m_distance;
}

void Plane::normalize()
{
  float inv_length = 1.0f / glm::length(m_normal);

  m_normal *= inv_length;
  m_distance *= inv_length;
}

Plane Plane::fromUnnormalized(const glm::vec3& normal, float distance)
{
  Plane plane(normal, distance);
  plane.normalize();

  return plane;
}

Frustum::Frustum()
{

}

Frustum::Frustum(const std::array<Plane, 6>& planes)
  : m_planes(planes)
{

}

const Plane& Frustum::getPlane(size_t index) const
{
  return m_planes[index];
}

Plane& Frustum::getPlane(size_t index)
{
  return m_planes[index];
}

const Plane& Frustum::getPlane(FrustumPlane plane) const
{
  return m_planes[static_cast<size_t>(plane)];
}

Plane& Frustum::getPlane(FrustumPlane plane)
{
  return m_planes[static_cast<size_t>(plane)];
}

void Frustum::setPlane(size_t index, const Plane& plane)
{
  m_planes[index] = plane;
}

void Frustum::setPlane(FrustumPlane planeType, const Plane& plane)
{
  m_planes[static_cast<size_t>(planeType)] = plane;
}

std::array<glm::vec3, 8> Frustum::getCorners() const
{
  return {
    GeometryUtils::getPlanesIntersection(getPlane(FrustumPlane::Near), getPlane(FrustumPlane::Left), getPlane(FrustumPlane::Bottom)),
    GeometryUtils::getPlanesIntersection(getPlane(FrustumPlane::Near), getPlane(FrustumPlane::Left), getPlane(FrustumPlane::Top)),
    GeometryUtils::getPlanesIntersection(getPlane(FrustumPlane::Near), getPlane(FrustumPlane::Right), getPlane(FrustumPlane::Top)),
    GeometryUtils::getPlanesIntersection(getPlane(FrustumPlane::Near),
      getPlane(FrustumPlane::Right),
      getPlane(FrustumPlane::Bottom)),
    GeometryUtils::getPlanesIntersection(getPlane(FrustumPlane::Far), getPlane(FrustumPlane::Left), getPlane(FrustumPlane::Bottom)),
    GeometryUtils::getPlanesIntersection(getPlane(FrustumPlane::Far), getPlane(FrustumPlane::Left), getPlane(FrustumPlane::Top)),
    GeometryUtils::getPlanesIntersection(getPlane(FrustumPlane::Far), getPlane(FrustumPlane::Right), getPlane(FrustumPlane::Top)),
    GeometryUtils::getPlanesIntersection(getPlane(FrustumPlane::Far), getPlane(FrustumPlane::Right), getPlane(FrustumPlane::Bottom)),
  };
}

Frustum Frustum::extractFromViewProjection(const glm::mat4x4& view, const glm::mat4x4 projection)
{
  glm::mat4x4 viewProjection = projection * view;

  Frustum frustum;

  frustum.setPlane(FrustumPlane::Left, Plane(glm::vec3(
    viewProjection[0][3] + viewProjection[0][0],
    viewProjection[1][3] + viewProjection[1][0],
    viewProjection[2][3] + viewProjection[2][0]),
    viewProjection[3][3] + viewProjection[3][0]));

  frustum.setPlane(FrustumPlane::Right, Plane(glm::vec3(
    viewProjection[0][3] - viewProjection[0][0],
    viewProjection[1][3] - viewProjection[1][0],
    viewProjection[2][3] - viewProjection[2][0]),
    viewProjection[3][3] - viewProjection[3][0]));

  frustum.setPlane(FrustumPlane::Top, Plane(glm::vec3(
    viewProjection[0][3] - viewProjection[0][1],
    viewProjection[1][3] - viewProjection[1][1],
    viewProjection[2][3] - viewProjection[2][1]),
    viewProjection[3][3] - viewProjection[3][1]));

  frustum.setPlane(FrustumPlane::Bottom, Plane(glm::vec3(
    viewProjection[0][3] + viewProjection[0][1],
    viewProjection[1][3] + viewProjection[1][1],
    viewProjection[2][3] + viewProjection[2][1]),
    viewProjection[3][3] + viewProjection[3][1]));

  frustum.setPlane(FrustumPlane::Near, Plane(glm::vec3(
    viewProjection[0][3] + viewProjection[0][2],
    viewProjection[1][3] + viewProjection[1][2],
    viewProjection[2][3] + viewProjection[2][2]),
    viewProjection[3][3] + viewProjection[3][2]));

  frustum.setPlane(FrustumPlane::Far, Plane(glm::vec3(
    viewProjection[0][3] - viewProjection[0][2],
    viewProjection[1][3] - viewProjection[1][2],
    viewProjection[2][3] - viewProjection[2][2]),
    viewProjection[3][3] - viewProjection[3][2]));

  for (size_t planeIndex = 0; planeIndex < 6; planeIndex++) {
    frustum.getPlane(planeIndex).normalize();
  }

  return frustum;
}

Sphere::Sphere()
{

}

Sphere::Sphere(const glm::vec3& origin, float radius)
  : m_origin(origin), m_radius(radius)
{

}

void Sphere::setOrigin(const glm::vec3& origin)
{
  m_origin = origin;
}

glm::vec3 Sphere::getOrigin() const
{
  return m_origin;
}

void Sphere::setRadius(float radius)
{
  m_radius = radius;
}

float Sphere::getRadius() const
{
  return m_radius;
}

float GeometryUtils::calculateDistance(const glm::vec3& v1, const glm::vec3& v2)
{
  return glm::distance(v1, v2);
}

float GeometryUtils::calculateDistance(const glm::vec3& point, const Plane& plane)
{
  return abs(GeometryUtils::calculateSignedDistance(point, plane));
}

float GeometryUtils::calculateSignedDistance(const glm::vec3& point, const Plane& plane)
{
  return glm::dot(plane.getNormal(), point) + plane.getDistance();
}

bool GeometryUtils::isSphereFrustumIntersecting(const Sphere& sphere, const Frustum& frustum)
{
  for (size_t sideIndex = 0; sideIndex < 6; sideIndex++) {
    const Plane& plane = frustum.getPlane(sideIndex);

    if (calculateSignedDistance(sphere.getOrigin(), plane) < -sphere.getRadius()) {
      return false;
    }
  }

  return true;
}

glm::vec3 GeometryUtils::getPlanesIntersection(const Plane& p1, const Plane& p2, const Plane& p3)
{
  glm::mat3 normalMatrix(p1.getNormal(), p2.getNormal(), p3.getNormal());
  glm::mat3 inversedNormalMatrix(glm::inverse(normalMatrix));

  return glm::vec3(-p1.getDistance(), -p2.getDistance(), -p3.getDistance()) * inversedNormalMatrix;
}

AABB::AABB()
  : m_min(glm::vec3(0.0f, 0.0f, 0.0f)),
    m_max(glm::vec3(0.0f, 0.0f, 0.0f))
{

}

AABB::AABB(const glm::vec3& min, const glm::vec3& max)
  : m_min(min), m_max(max)
{

}

void AABB::setMin(const glm::vec3& min)
{
  m_min = min;
}

const glm::vec3& AABB::getMin() const
{
  return m_min;
}

void AABB::setMax(const glm::vec3& max)
{
  m_max = max;
}

const glm::vec3& AABB::getMax() const
{
  return m_max;
}

glm::vec3 AABB::getSize() const
{
  return m_max - m_min;
}

Sphere AABB::toSphere() const
{
  glm::vec3 size = getSize();
  float radius = glm::length(size) * 0.5f;
  glm::vec3 origin = (m_max + m_min) * 0.5f;

  return Sphere(origin, radius);
}

std::array<glm::vec3, 8> AABB::getCorners() const
{
  return {
    glm::vec3{m_min.x, m_min.y, m_min.z},
    glm::vec3{m_max.x, m_min.y, m_min.z},
    glm::vec3{m_min.x, m_max.y, m_min.z},
    glm::vec3{m_min.x, m_min.y, m_max.z},

    glm::vec3{m_max.x, m_max.y, m_max.z},
    glm::vec3{m_min.x, m_max.y, m_max.z},
    glm::vec3{m_max.x, m_min.y, m_max.z},
    glm::vec3{m_max.x, m_max.y, m_min.z},
  };
}

bool GeometryUtils::isAABBFrustumIntersecting(const AABB& aabb, const Frustum& frustum)
{
  const auto& corners = aabb.getCorners();

  for (size_t sideIndex = 0; sideIndex < 6; sideIndex++) {
    const Plane& plane = frustum.getPlane(sideIndex);

    size_t pointsOutsideFrustum = 0;

    for (const glm::vec3& corner : corners) {
      pointsOutsideFrustum += calculateSignedDistance(corner, plane) < 0.0f;
    }

    if (pointsOutsideFrustum == 8) {
      return false;
    }
  }

  return true;
}
