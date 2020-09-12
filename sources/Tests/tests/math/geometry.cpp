#include <catch2/catch.hpp>

#include <Engine/Modules/Math/geometry.h>
#include <Engine/Modules/Math/MathUtils.h>


TEST_CASE("distances_points", "[math]") {
    glm::vec3 pointA = {10.0f, 0.0f, 0.0f};
    glm::vec3 pointB = {20.0f, 0.0f, 0.0f};

    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateDistance(pointA, pointB), 10.0f));
}

TEST_CASE("absolute_distances_point_plane", "[math]") {
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

TEST_CASE("signed_distances_point_plane", "[math]") {
    glm::vec3 point = {0.0f, 10.0f, 0.0f};
    Plane plane = Plane({0.0f, 1.0f, 0.0f}, 0.0f);

    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateSignedDistance(point, plane), 10.0f));
    REQUIRE(MathUtils::isEqual(GeometryUtils::calculateSignedDistance(-point, plane), -10.0f));
}

TEST_CASE("intersection_point_3_planes", "[math]") {
    Plane p1 = Plane::fromUnnormalized(glm::vec3{1.0f, 1.0f, 1.0f}, -1.0f);
    Plane p2 = Plane::fromUnnormalized(glm::vec3{1.0f, -2.0f, 0.0f}, 0.0f);
    Plane p3 = Plane::fromUnnormalized(glm::vec3{2.0f, 1.0f, 3.0f}, 1.0f);

    REQUIRE(MathUtils::isEqual(GeometryUtils::getPlanesIntersection(p1, p2, p3), glm::vec3{2.0f, 1.0f, -2.0f}));
}
