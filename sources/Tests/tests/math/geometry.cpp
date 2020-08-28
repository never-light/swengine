#include <catch2/catch.hpp>

#include <glm/gtx/string_cast.hpp>

#include <Engine/Modules/Math/geometry.h>
#include <Engine/Modules/Math/MathUtils.h>


TEST_CASE("Distances", "[geometry]")
{
  SECTION("Points") {
    glm::vec3 pointA = {10.0f, 0.0f, 0.0f};
    glm::vec3 pointB = {20.0f, 0.0f, 0.0f};

    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateDistance(pointA, pointB), 10.0f));
  }

  SECTION("Point and plane, absolute") {
    glm::vec3 point = {0.0f, 10.0f, 0.0f};
    Plane plane = Plane({0.0f, 1.0f, 0.0f}, 0.0f);

    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateDistance(point, plane), 10.0f));
  }

  SECTION("Point and plane, absolute") {
    glm::vec3 point = {1.0f, 2.0f, 3.0f};
    Plane plane = Plane::fromUnnormalized(glm::vec3{2.0f, 3.0f, 4.0f}, 5.0f);

    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateDistance(point, plane), 4.6423834f));
  }

  SECTION("Point and plane, signed") {
    glm::vec3 point = {0.0f, 10.0f, 0.0f};
    Plane plane = Plane({0.0f, 1.0f, 0.0f}, 0.0f);

    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateSignedDistance(point, plane), 10.0f));
    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateSignedDistance(-point, plane), -10.0f));
  }
}
