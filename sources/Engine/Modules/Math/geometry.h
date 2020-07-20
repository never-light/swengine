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
  [[nodiscard]] glm::vec3 getNormal() const;

  void setDistance(float distance);
  [[nodiscard]] float getDistance() const;

  void normalize();

  [[nodiscard]] static Plane fromUnnormalized(const glm::vec3& normal, float distance);

 private:
  glm::vec3 m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
  float m_distance = 0.0f;
};

enum class FrustumPlane {
  Left,
  Right,
  Top,
  Bottom,
  Near,
  Far
};

struct Frustum {
 public:
  Frustum();
  explicit Frustum(const std::array<Plane, 6>& planes);

  [[nodiscard]] const Plane& getPlane(size_t index) const;
  [[nodiscard]] Plane& getPlane(size_t index);

  [[nodiscard]] const Plane& getPlane(FrustumPlane plane) const;
  [[nodiscard]] Plane& getPlane(FrustumPlane plane);

  void setPlane(size_t index, const Plane& plane);
  void setPlane(FrustumPlane planeType, const Plane& plane);

  [[nodiscard]] std::array<glm::vec3, 8> getCorners() const;

 public:
  static Frustum extractFromViewProjection(const glm::mat4x4& view, const glm::mat4x4 projection);

 private:
  // Left, right, top, bottom, near, far
  std::array<Plane, 6> m_planes;
};

struct Sphere {
 public:
  Sphere();
  Sphere(const glm::vec3& origin, float radius);

  void setOrigin(const glm::vec3& origin);
  [[nodiscard]] glm::vec3 getOrigin() const;

  void setRadius(float radius);
  [[nodiscard]] float getRadius() const;

 private:
  glm::vec3 m_origin = glm::vec3(0.0f, 0.0f, 0.0f);
  float m_radius = 0.0f;
};

struct AABB {
 public:
  AABB();
  AABB(const glm::vec3& min, const glm::vec3& max);

  void setMin(const glm::vec3& min);
  [[nodiscard]] const glm::vec3& getMin() const;

  void setMax(const glm::vec3& max);
  [[nodiscard]] const glm::vec3& getMax() const;

  [[nodiscard]] glm::vec3 getSize() const;

  [[nodiscard]] Sphere toSphere() const;

  [[nodiscard]] std::array<glm::vec3, 8> getCorners() const;

 private:
  glm::vec3 m_min;
  glm::vec3 m_max;
};

class GeometryUtils {
 public:
  static float calculateDistance(const glm::vec3& v1, const glm::vec3& v2);
  static float calculateDistance(const glm::vec3& point, const Plane& plane);
  static float calculateSignedDistance(const glm::vec3& point, const Plane& plane);

  static glm::vec3 getPlanesIntersection(const Plane& p1, const Plane& p2, const Plane& p3);

  static bool isSphereFrustumIntersecting(const Sphere& sphere, const Frustum& frustum);
  static bool isAABBFrustumIntersecting(const AABB& aabb, const Frustum& frustum);
};

