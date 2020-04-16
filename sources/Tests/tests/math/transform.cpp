#include <catch2/catch.hpp>

#include <Engine/Modules/Graphics/GraphicsSystem/Transform.h>
#include <Engine/Modules/Math/matrices.h>

#include "utils/utils.h"

TEST_CASE( "Test affine transformations", "[transform]" ) {
    using tests::MathUtils;

    Transform transform;

    SECTION("Default transform") {
       REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(), MathUtils::IDENTITY_MATRIX4));

       REQUIRE(MathUtils::isEqual(transform.getFrontDirection(), -MathUtils::AXIS_Z));
       REQUIRE(MathUtils::isEqual(transform.getRightDirection(), MathUtils::AXIS_X));
       REQUIRE(MathUtils::isEqual(transform.getUpDirection(), MathUtils::AXIS_Y));
    }

    SECTION("Positions and movements") {
       transform.setPosition(10.0f, -15.0f, 20.0f);

       REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(),
                                  MathUtils::getTranslationMatix({ 10.0f, -15.0f, 20.0f })));

       transform.move(5.0f, 10.0f, -15.0f);

       REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(),
                                  MathUtils::getTranslationMatix({ 15.0f, -5.0f, 5.0f })));
    }

    SECTION("Rotation X") {
        transform.pitchGlobal(90.0f);

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(),
                                   MathUtils::getRotationMatrix(MathUtils::AXIS_X, 90.0f)));

        transform.pitchGlobal(-180.0f);

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(),
                                   MathUtils::getRotationMatrix(MathUtils::AXIS_X, -90.0f)));
    }

    SECTION("Rotation Y") {
        transform.yawGlobal(90.0f);

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(),
                                   MathUtils::getRotationMatrix(MathUtils::AXIS_Y, 90.0f)));

        transform.yawGlobal(-180.0f);

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(),
                                   MathUtils::getRotationMatrix(MathUtils::AXIS_Y, -90.0f)));
    }

    SECTION("Rotation Z") {
        transform.rollGlobal(90.0f);

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(),
                                   MathUtils::getRotationMatrix(MathUtils::AXIS_Z, 90.0f)));

        transform.rollGlobal(-180.0f);

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(),
                                   MathUtils::getRotationMatrix(MathUtils::AXIS_Z, -90.0f)));
    }

    SECTION("Combine rotations") {
        transform.pitchGlobal(45.0f);
        transform.yawGlobal(-90.0f);
        transform.rollGlobal(-180.0f);

        glm::mat4 rotationMatrix = MathUtils::getRollMatrix(-180.0f) *
                MathUtils::getYawMatrix(-90.0f) *
                MathUtils::getPitchMatrix(45.0f);

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(), rotationMatrix));
    }

    SECTION("Scale") {
        transform.scale({ 1.0f, 2.0f, 3.0f });

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(),
                                   MathUtils::getScaleMatrix({ 1.0f, 2.0f, 3.0f })));
    }

    SECTION("Combine scale, position") {
        transform.setScale({ 2.0f, 3.0f, 4.0f });
        transform.setPosition({ 10.0f, 20.0f, 30.0f });

        glm::mat4 resultMatrix = MathUtils::getTranslationMatix({ 10.0f, 20.0f, 30.0f }) *
                MathUtils::getScaleMatrix({ 2.0f, 3.0f, 4.0f });

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(), resultMatrix));
    }

    SECTION("Combine scale, rotation, position") {
        transform.setScale({ 2.0f, 3.0f, 4.0f });
        transform.setPosition({ 10.0f, 20.0f, 30.0f });
        transform.pitchGlobal(45.0f);
        transform.yawGlobal(45.0f);

        glm::mat4 resultMatrix = MathUtils::getTranslationMatix({ 10.0f, 20.0f, 30.0f }) *
                MathUtils::getRotationMatrixFromYawPitchRoll(45.0f, 45.0f, 0.0f) *
                MathUtils::getScaleMatrix({ 2.0f, 3.0f, 4.0f });

        REQUIRE(MathUtils::isEqual(transform.getTransformationMatrix(), resultMatrix));
    }

    SECTION("Look directions") {
        transform.yawGlobal(180.0f);

        REQUIRE(MathUtils::isEqual(transform.getFrontDirection(), MathUtils::AXIS_Z));
        REQUIRE(MathUtils::isEqual(transform.getRightDirection(), -MathUtils::AXIS_X));
        REQUIRE(MathUtils::isEqual(transform.getUpDirection(), MathUtils::AXIS_Y));

        transform.pitchGlobal(90.0f);

        REQUIRE(MathUtils::isEqual(transform.getFrontDirection(), -MathUtils::AXIS_Y));
        REQUIRE(MathUtils::isEqual(transform.getUpDirection(), MathUtils::AXIS_Z));
        REQUIRE(MathUtils::isEqual(transform.getRightDirection(), -MathUtils::AXIS_X));
    }
}
