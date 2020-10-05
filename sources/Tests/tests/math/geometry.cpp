#include <catch2/catch.hpp>

#include <array>

#include <Engine/Modules/Math/geometry.h>
#include <Engine/Modules/Math/MathUtils.h>

TEST_CASE("distances_points", "[math]")
{
  glm::vec3 pointA = {10.0f, 0.0f, 0.0f};
  glm::vec3 pointB = {20.0f, 0.0f, 0.0f};

  REQUIRE(MathUtils::isEqual(GeometryUtils::calculateDistance(pointA, pointB), 10.0f));
}

TEST_CASE("absolute_distances_point_plane", "[math]")
{
  SECTION("case_1") {
    glm::vec3 point = {0.0f, 10.0f, 0.0f};
    Plane plane = Plane({0.0f, 1.0f, 0.0f}, 0.0f);

    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateDistance(point, plane), 10.0f));
  }

  SECTION("case_2") {
    glm::vec3 point = {1.0f, 2.0f, 3.0f};
    Plane plane = Plane::fromUnnormalized(glm::vec3{2.0f, 3.0f, 4.0f}, 5.0f);

    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateDistance(point, plane), 4.6423834f));
  }
}

TEST_CASE("signed_distances_point_plane", "[math]")
{
  glm::vec3 point = {0.0f, 10.0f, 0.0f};
  Plane plane = Plane({0.0f, 1.0f, 0.0f}, 0.0f);

  REQUIRE(MathUtils::isEqual(GeometryUtils::calculateSignedDistance(point, plane), 10.0f));
  REQUIRE(MathUtils::isEqual(GeometryUtils::calculateSignedDistance(-point, plane), -10.0f));
}

TEST_CASE("intersection_point_3_planes", "[math]")
{
  Plane p1 = Plane::fromUnnormalized(glm::vec3{1.0f, 1.0f, 1.0f}, -1.0f);
  Plane p2 = Plane::fromUnnormalized(glm::vec3{1.0f, -2.0f, 0.0f}, 0.0f);
  Plane p3 = Plane::fromUnnormalized(glm::vec3{2.0f, 1.0f, 3.0f}, 1.0f);

  REQUIRE(MathUtils::isEqual(GeometryUtils::getPlanesIntersection(p1, p2, p3), glm::vec3{2.0f, 1.0f, -2.0f}));
}

TEST_CASE("frustum_extract_from_corners", "[math]")
{
  std::array<glm::vec3, 8> corners = {glm::vec3{2.0f, 0.0f, 2.0f},
    glm::vec3{-2.0f, 0.0f, 2.0f},
    glm::vec3{-2.0f, 0.0f, -2.0f},
    glm::vec3{2.0f, 0.0f, -2.0f},
    glm::vec3{4.0f, 4.0f, 4.0f},
    glm::vec3{-4.0f, 4.0f, 4.0f},
    glm::vec3{-4.0f, 4.0f, -4.0f},
    glm::vec3{4.0f, 4.0f, -4.0f}};

  Frustum frustum = Frustum::extractFromCorners(corners);

  // Left
  REQUIRE(MathUtils::isEqual(frustum.getPlane(0),
    GeometryUtils::getPlaneBy3Points(corners[7], corners[3], corners[0])));
  REQUIRE(MathUtils::isEqual(frustum.getPlane(0).getInverse(),
    GeometryUtils::getPlaneBy3Points(corners[0], corners[3], corners[7])));

  // Right
  REQUIRE(MathUtils::isEqual(frustum.getPlane(1),
    GeometryUtils::getPlaneBy3Points(corners[6], corners[5], corners[1])));

  REQUIRE(MathUtils::isEqual(frustum.getPlane(1).getInverse(),
    GeometryUtils::getPlaneBy3Points(corners[1], corners[5], corners[6])));

  // Top
  REQUIRE(MathUtils::isEqual(frustum.getPlane(2),
    GeometryUtils::getPlaneBy3Points(corners[3], corners[7], corners[6])));

  REQUIRE(MathUtils::isEqual(frustum.getPlane(2).getInverse(),
    GeometryUtils::getPlaneBy3Points(corners[6], corners[7], corners[3])));

  // Bottom
  REQUIRE(MathUtils::isEqual(frustum.getPlane(3),
    GeometryUtils::getPlaneBy3Points(corners[4], corners[0], corners[1])));

  REQUIRE(MathUtils::isEqual(frustum.getPlane(3).getInverse(),
    GeometryUtils::getPlaneBy3Points(corners[1], corners[0], corners[4])));

  // Near
  REQUIRE(MathUtils::isEqual(frustum.getPlane(4),
    GeometryUtils::getPlaneBy3Points(corners[3], corners[2], corners[1])));

  REQUIRE(MathUtils::isEqual(frustum.getPlane(4).getInverse(),
    GeometryUtils::getPlaneBy3Points(corners[1], corners[2], corners[3])));

  // Far
  REQUIRE(MathUtils::isEqual(frustum.getPlane(5),
    GeometryUtils::getPlaneBy3Points(corners[4], corners[5], corners[6])));

  REQUIRE(MathUtils::isEqual(frustum.getPlane(5).getInverse(),
    GeometryUtils::getPlaneBy3Points(corners[6], corners[5], corners[4])));
}

TEST_CASE("frustum_aabb_intersection", "[math]")
{
  Frustum frustum = Frustum::extractFromCorners({glm::vec3{2.0f, 0.0f, 2.0f},
    glm::vec3{-2.0f, 0.0f, 2.0f},
    glm::vec3{-2.0f, 0.0f, -2.0f},
    glm::vec3{2.0f, 0.0f, -2.0f},
    glm::vec3{4.0f, 4.0f, 4.0f},
    glm::vec3{-4.0f, 4.0f, 4.0f},
    glm::vec3{-4.0f, 4.0f, -4.0f},
    glm::vec3{4.0f, 4.0f, -4.0f}});

  SECTION("inside") {
    AABB aabb(glm::vec3{0.0f, 1.0f, 0.0}, glm::vec3{1.0f, 2.0f, 1.0f});
    REQUIRE(MathUtils::isEqual(GeometryUtils::isAABBFrustumIntersecting(aabb, frustum), true));
  }

  SECTION("partial") {
    AABB aabb(glm::vec3{-1.0f, 1.0f, -1.0}, glm::vec3{3.0f, 5.0f, 3.0f});
    REQUIRE(MathUtils::isEqual(GeometryUtils::isAABBFrustumIntersecting(aabb, frustum), true));
  }

  SECTION("outside") {
    AABB aabb(glm::vec3{-3.0f, -5.0f, -3.0f}, glm::vec3{-1.0f, -1.0f, -1.0f});
    REQUIRE(MathUtils::isEqual(GeometryUtils::isAABBFrustumIntersecting(aabb, frustum), false));
  }
}

TEST_CASE("frustum_sphere_intersection", "[math]")
{
  Frustum frustum = Frustum::extractFromCorners({glm::vec3{2.0f, 0.0f, 2.0f},
    glm::vec3{-2.0f, 0.0f, 2.0f},
    glm::vec3{-2.0f, 0.0f, -2.0f},
    glm::vec3{2.0f, 0.0f, -2.0f},
    glm::vec3{4.0f, 4.0f, 4.0f},
    glm::vec3{-4.0f, 4.0f, 4.0f},
    glm::vec3{-4.0f, 4.0f, -4.0f},
    glm::vec3{4.0f, 4.0f, -4.0f}});

  SECTION("partial") {
    Sphere sphere(glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f);
    REQUIRE(MathUtils::isEqual(GeometryUtils::isSphereFrustumIntersecting(sphere, frustum), true));
  }

  SECTION("inside") {
    Sphere sphere(glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f);
    REQUIRE(MathUtils::isEqual(GeometryUtils::isSphereFrustumIntersecting(sphere, frustum), true));
  }

  SECTION("outside") {
    Sphere sphere(glm::vec3{0.0f, -3.0f, 0.0f}, 1.0f);
    REQUIRE(MathUtils::isEqual(GeometryUtils::isSphereFrustumIntersecting(sphere, frustum), false));
  }
}

TEST_CASE("frustum_corners_extraction", "[math]")
{
  std::array<glm::vec3, 8> corners = {glm::vec3{2.0f, 0.0f, 2.0f},
    glm::vec3{-2.0f, 0.0f, 2.0f},
    glm::vec3{-2.0f, 0.0f, -2.0f},
    glm::vec3{2.0f, 0.0f, -2.0f},
    glm::vec3{4.0f, 4.0f, 4.0f},
    glm::vec3{-4.0f, 4.0f, 4.0f},
    glm::vec3{-4.0f, 4.0f, -4.0f},
    glm::vec3{4.0f, 4.0f, -4.0f}};

  Frustum frustum = Frustum::extractFromCorners(corners);

  std::array<glm::vec3, 8> extractedCorners = frustum.getCorners();

  for (size_t cornerIndex = 0; cornerIndex < 8; cornerIndex++) {
    REQUIRE(MathUtils::isEqual(corners[cornerIndex], extractedCorners[cornerIndex]));
  }
}

