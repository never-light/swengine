#include <catch2/catch.hpp>

#include <Engine/Modules/Math/geometry.h>
#include <Engine/Modules/Math/MathUtils.h>

#include <Engine/Utility/memory.h>

#include <Engine/Modules/Graphics/Resources/Raw/RawMesh.h>


TEST_CASE("сolliders_parameters_restoring_AABB", "[math]") {
    auto rawMesh = RawMesh::readFromFile("../resources/models/primitives/box.mesh");

    std::vector<glm::vec3> positions = MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawMesh.positions);
    AABB aabb = restoreAABBByVerticesList(positions);

    REQUIRE(MathUtils::isEqual(aabb.getMax(), {0.5f, 0.5f, 0.5f}, 1e-2f ));
    REQUIRE(MathUtils::isEqual(aabb.getMin(), {-0.5f, -0.5f, -0.5f}, 1e-2f ));
}

TEST_CASE("сolliders_parameters_restoring_sphere", "[math]") {
    auto rawMesh = RawMesh::readFromFile("../resources/models/primitives/sphere.mesh");

    std::vector<glm::vec3> positions = MemoryUtils::createBinaryCompatibleVector<RawVector3, glm::vec3>(rawMesh.positions);
    Sphere sphere = restoreSphereByVerticesList(positions);

    REQUIRE(MathUtils::isEqual(sphere.getOrigin(), {0.0f, 0.0f, 0.0f}, 1e-2f ));
    REQUIRE(MathUtils::isEqual(sphere.getRadius(), 1.0f, 1e-2f ));
}