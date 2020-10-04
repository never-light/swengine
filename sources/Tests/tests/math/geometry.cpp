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

TEST_CASE("frustum_extract_from_corners", "[math]") {
    Frustum frustum = Frustum::extractFromCorners({glm::vec3{2.0f, 0.0f, 2.0f},
                                                   glm::vec3{-2.0f, 0.0f, 2.0f},
                                                   glm::vec3{-2.0f, 0.0f, -2.0f},
                                                   glm::vec3{2.0f, 0.0f, -2.0f},
                                                   glm::vec3{4.0f, 4.0f, 4.0f},
                                                   glm::vec3{-4.0f, 4.0f, 4.0f},
                                                   glm::vec3{-4.0f, 4.0f, -4.0f},
                                                   glm::vec3{4.0f, 4.0f, -4.0f}});

    REQUIRE(MathUtils::isEqual(frustum.getPlane(0), GeometryUtils::getPlaneBy3Points(glm::vec3{2.0f, 0.0f, 2.0f},
                                                                                     glm::vec3{2.0f, 0.0f, -2.0f},
                                                                                     glm::vec3{4.0f, 4.0f, -4.0f})));
    REQUIRE(MathUtils::isEqual(frustum.getPlane(1), GeometryUtils::getPlaneBy3Points(glm::vec3{-2.0f, 0.0f, 2.0f},
                                                                                     glm::vec3{-2.0f, 0.0f, -2.0f},
                                                                                     glm::vec3{-4.0f, 4.0f, 4.0f})));
    REQUIRE(MathUtils::isEqual(frustum.getPlane(2), GeometryUtils::getPlaneBy3Points(glm::vec3{2.0f, 0.0f, 2.0f},
                                                                                     glm::vec3{-2.0f, 0.0f, 2.0f},
                                                                                     glm::vec3{-4.0f, 4.0f, 4.0f})));
    REQUIRE(MathUtils::isEqual(frustum.getPlane(3), GeometryUtils::getPlaneBy3Points(glm::vec3{-2.0f, 0.0f, -2.0f},
                                                                                     glm::vec3{2.0f, 0.0f, -2.0f},
                                                                                     glm::vec3{-4.0f, 4.0f, -4.0f})));
    REQUIRE(MathUtils::isEqual(frustum.getPlane(4), GeometryUtils::getPlaneBy3Points(glm::vec3{2.0f, 0.0f, 2.0f},
                                                                                     glm::vec3{-2.0f, 0.0f, 2.0f},
                                                                                     glm::vec3{-2.0f, 0.0f, -2.0f})));
    REQUIRE(MathUtils::isEqual(frustum.getPlane(5), GeometryUtils::getPlaneBy3Points(glm::vec3{-4.0f, 4.0f, 4.0f},
                                                                                     glm::vec3{-4.0f, 4.0f, -4.0f},
                                                                                     glm::vec3{4.0f, 4.0f, -4.0f})));

}

TEST_CASE("frustum_aabb_intersection", "[math]") {
    Frustum frustum = Frustum::extractFromCorners({glm::vec3{2.0f, 0.0f, 2.0f},
                                                   glm::vec3{-2.0f, 0.0f, 2.0f},
                                                   glm::vec3{-2.0f, 0.0f, -2.0f},
                                                   glm::vec3{2.0f, 0.0f, -2.0f},
                                                   glm::vec3{4.0f, 4.0f, 4.0f},
                                                   glm::vec3{-4.0f, 4.0f, 4.0f},
                                                   glm::vec3{-4.0f, 4.0f, -4.0f},
                                                   glm::vec3{4.0f, 4.0f, -4.0f}});
    AABB aabb1 = AABB(glm::vec3{0.0f, 1.0f, 0.0}, glm::vec3{1.0f, 2.0f, 1.0f});
    AABB aabb2 = AABB(glm::vec3{-1.0f, 1.0f, -1.0}, glm::vec3{3.0f, 5.0f, 3.0f});
    AABB aabb3 = AABB(glm::vec3{-3.0f, -5.0f, -3.0f}, glm::vec3{-1.0f, -1.0f, -1.0});

    REQUIRE(MathUtils::isEqual(GeometryUtils::isAABBFrustumIntersecting(aabb1, frustum), true));
    REQUIRE(MathUtils::isEqual(GeometryUtils::isAABBFrustumIntersecting(aabb2, frustum), true));
    REQUIRE(MathUtils::isEqual(GeometryUtils::isAABBFrustumIntersecting(aabb3, frustum), false));

}

TEST_CASE("frustum_sphere_intersection", "[math]") {
    Frustum frustum = Frustum::extractFromCorners({glm::vec3{2.0f, 0.0f, 2.0f},
                                                   glm::vec3{-2.0f, 0.0f, 2.0f},
                                                   glm::vec3{-2.0f, 0.0f, -2.0f},
                                                   glm::vec3{2.0f, 0.0f, -2.0f},
                                                   glm::vec3{4.0f, 4.0f, 4.0f},
                                                   glm::vec3{-4.0f, 4.0f, 4.0f},
                                                   glm::vec3{-4.0f, 4.0f, -4.0f},
                                                   glm::vec3{4.0f, 4.0f, -4.0f}});

    Sphere sphere1 = Sphere(glm::vec3{0.0f, 1.0f, 0.0f}, 1.0f);
    Sphere sphere2 = Sphere(glm::vec3{0.0f, 0.0f, 0.0f}, 1.0f);
    Sphere sphere3 = Sphere(glm::vec3{0.0f, -3.0f, 0.0f}, 1.0f);

    REQUIRE(MathUtils::isEqual(GeometryUtils::isSphereFrustumIntersecting(sphere1, frustum), true));
    REQUIRE(MathUtils::isEqual(GeometryUtils::isSphereFrustumIntersecting(sphere2, frustum), true));
    REQUIRE(MathUtils::isEqual(GeometryUtils::isSphereFrustumIntersecting(sphere3, frustum), false));

}


