#pragma once

#include <glm/vec3.hpp>
#include <cmath>
#include <array>

struct Plane {
public:
    Plane(const glm::vec3& normal, float distance);

    void setNormal(const glm::vec3& normal);
    glm::vec3 getNormal() const;

    void setDistance(float distance);
    float getDistance() const;

private:
    glm::vec3 m_normal;
    float m_distance;
};

struct Frustum {
public:
    Frustum(const std::array<Plane, 6>& planes);

    const Plane& getPlane(size_t index) const;

    const Plane& operator[](size_t index) const;
    Plane& operator[](size_t index);

private:
    std::array<Plane, 6> m_planes;
};

struct Sphere {
public:
    Sphere(const glm::vec3& origin, float radius);

    void setOrigin(const glm::vec3& origin);
    glm::vec3 getOrigin() const;

    void setRadius(float radius);
    float getRadius() const;

private:
    glm::vec3 m_origin;
    float m_radius;
};

float calculateDistance(const glm::vec3& v1, const glm::vec3& v2);
float calculateDistance(const glm::vec3& point, const Plane& plane);
float calculateSignedDistance(const glm::vec3& point, const Plane& plane);

bool isSphereFrustumIntersecting(const Sphere& sphere, const Frustum& frustum);
