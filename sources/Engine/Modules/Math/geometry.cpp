#include "geometry.h"

#include <glm/geometric.hpp>

Plane::Plane()
{

}

Plane::Plane(const glm::vec3& normal, float distance)
    : m_normal(glm::normalize(normal)), m_distance(distance)
{

}

void Plane::setNormal(const glm::vec3& normal)
{
    m_normal = glm::normalize(normal);
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

const Plane& Frustum::operator[](size_t index) const
{
    return m_planes[index];
}

Plane& Frustum::operator[](size_t index)
{
    return m_planes[index];
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

float calculateDistance(const glm::vec3& v1, const glm::vec3& v2)
{
    return glm::distance(v1, v2);
}

float calculateDistance(const glm::vec3& point, const Plane& plane)
{
    return abs(calculateSignedDistance(point, plane));
}

float calculateSignedDistance(const glm::vec3& point, const Plane& plane)
{
    return glm::dot(plane.getNormal(), point) + plane.getDistance();
}

bool isSphereFrustumIntersecting(const Sphere& sphere, const Frustum& frustum)
{
    for (size_t sideIndex = 0; sideIndex < 6; sideIndex++) {
        const Plane& plane = frustum.getPlane(sideIndex);

        if (calculateSignedDistance(sphere.getOrigin(), plane) < -sphere.getRadius()) {
            return false;
        }
    }

    return true;
}

Frustum extractFrustumFromViewProjection(const glm::mat4x4& view, const glm::mat4x4 projection)
{
    ARG_UNUSED(view);
    ARG_UNUSED(projection);

    return {};
}
