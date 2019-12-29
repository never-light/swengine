#pragma once

#include <cmath>
#include <array>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Plane {
public:
    Plane();
    Plane(const glm::vec3& normal, float distance);

    void setNormal(const glm::vec3& normal);
    glm::vec3 getNormal() const;

    void setDistance(float distance);
    float getDistance() const;

private:
    glm::vec3 m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
    float m_distance = 0.0f;
};

struct Frustum {
public:
    Frustum();
    Frustum(const std::array<Plane, 6>& planes);

    const Plane& getPlane(size_t index) const;

    const Plane& operator[](size_t index) const;
    Plane& operator[](size_t index);

private:
    std::array<Plane, 6> m_planes;
};

struct Sphere {
public:
    Sphere();
    Sphere(const glm::vec3& origin, float radius);

    void setOrigin(const glm::vec3& origin);
    glm::vec3 getOrigin() const;

    void setRadius(float radius);
    float getRadius() const;

private:
    glm::vec3 m_origin = glm::vec3(0.0f, 0.0f, 0.0f);
    float m_radius = 0.0f;
};

float calculateDistance(const glm::vec3& v1, const glm::vec3& v2);
float calculateDistance(const glm::vec3& point, const Plane& plane);
float calculateSignedDistance(const glm::vec3& point, const Plane& plane);

bool isSphereFrustumIntersecting(const Sphere& sphere, const Frustum& frustum);

Frustum extractFrustumFromViewProjection(const glm::mat4x4& view, const glm::mat4x4 projection);
