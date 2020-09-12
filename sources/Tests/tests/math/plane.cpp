#include <catch2/catch.hpp>

#include <Engine/Modules/Math/geometry.h>
#include <Engine/Modules/Math/MathUtils.h>


TEST_CASE("plane_by_3_points", "[math]") {
    glm::vec3 normal = {-3.0f * glm::inversesqrt(35.0f), glm::sqrt(5.0f / 7.0f), -1.0f * glm::inversesqrt(35.0f)};
    float d = -6.0f * glm::inversesqrt(35.0f);
    Plane plane = Plane(normal, d);

    REQUIRE(MathUtils::isEqual(
      GeometryUtils::getPlaneBy3Points({1.0f, 2.0f, 1.0f}, {2.0f, 3.0f, 3.0f}, {4.0f, 4.0f, 2.0f}), plane));
}
