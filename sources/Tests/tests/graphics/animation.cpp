#include "catch.hpp"

#include <Engine/Modules/Graphics/GraphicsSystem/Animation/Skeleton.h>
#include <Engine/Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationClip.h>

#include "utils/utils.h"

Skeleton generateTestSkeleton() {
    using tests::MathUtils;

    std::vector<Bone> bones;

    bones.push_back(Bone("root", 255, glm::inverse(MathUtils::getTranslationMatix({ 1.0f, 0.0f, 0.0f }))));
    bones.push_back(Bone("spin", 0, glm::inverse(MathUtils::getTranslationMatix({ 0.0f, 1.0f, 0.0f }))));
    bones.push_back(Bone("spin_child", 1, glm::inverse(MathUtils::getTranslationMatix({ 0.0f, 0.0f, 1.0f }))));

    Skeleton skeleton(bones);

    return skeleton;
}

SkeletalAnimationClip generateTestAnimationClip() {
    using tests::MathUtils;

    std::vector<BoneAnimationChannel> bonesAnimationChannels;

    bonesAnimationChannels.push_back(BoneAnimationChannel({
        BoneAnimationPositionFrame{ 0.0f, { 0.0f, 0.0f, 0.0f } },
        BoneAnimationPositionFrame{ 30.0f, { 30.0f, 0.0f, 0.0f } },
        BoneAnimationPositionFrame{ 60.0f, { 0.0f, 0.0f, 0.0f } }
    }, {}));

    bonesAnimationChannels.push_back(BoneAnimationChannel({
        BoneAnimationPositionFrame{ 0.0f, { 0.0f, 0.0f, 0.0f } },
        BoneAnimationPositionFrame{ 30.0f, { 00.0f, 30.0f, 0.0f } },
        BoneAnimationPositionFrame{ 60.0f, { 0.0f, 0.0f, 0.0f } }
    }, {
        BoneAnimationOrientationFrame{ 0.0f, glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X) },
        BoneAnimationOrientationFrame{ 30.0f, glm::angleAxis(glm::radians(90.0f), MathUtils::AXIS_X) },
        BoneAnimationOrientationFrame{ 60.0f, glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X) },
    }));

    bonesAnimationChannels.push_back(BoneAnimationChannel({}, {}));

    SkeletalAnimationClip animationClip = SkeletalAnimationClip("clip", 60.0f, 30.0f, bonesAnimationChannels);

    return animationClip;
}

TEST_CASE("bone-pose-matrix", "[animation]") {
    using tests::MathUtils;

    glm::vec3 originPosition = { 1.0f, 2.0f, 3.0f };
    glm::quat originOrientation = glm::angleAxis(glm::radians(45.0f), MathUtils::AXIS_X);
    BonePose originPose(originPosition, originOrientation);

    REQUIRE(MathUtils::isEqual(originPose.getBoneMatrix(), MathUtils::getTranslationMatix(originPosition) *
                               glm::mat4_cast(originOrientation)));
}

TEST_CASE("bone-poses-interpolation", "[animation]") {
    using tests::MathUtils;

    glm::vec3 originPosition = { 1.0f, 2.0f, 3.0f };
    glm::quat originOrientation = glm::angleAxis(glm::radians(45.0f), MathUtils::AXIS_X);
    BonePose originPose(originPosition, originOrientation);

    SECTION("interpolation") {
        glm::vec3 targetPosition = { 2.0f, 4.0f, 6.0f };
        glm::quat targetOrientation = glm::angleAxis(glm::radians(45.0f), MathUtils::AXIS_Y);

        BonePose targetPose(targetPosition, targetOrientation);

        float interpolationFactor = 0.5f;
        BonePose interpolatedPose = BonePose::interpolate(originPose, targetPose, interpolationFactor);

        glm::vec3 interpolatedPosition = glm::mix(originPosition, targetPosition, interpolationFactor);
        glm::quat interpolatedOrientation = glm::slerp(originOrientation, targetOrientation, interpolationFactor);

        REQUIRE(MathUtils::isEqual(interpolatedPose.position, interpolatedPosition));
        REQUIRE(MathUtils::isEqual(interpolatedPose.orientation, interpolatedOrientation));

        REQUIRE(MathUtils::isEqual(interpolatedPose.getBoneMatrix(), MathUtils::getTranslationMatix(interpolatedPosition) *
                                   glm::mat4_cast(interpolatedOrientation)));
    }
}

TEST_CASE("getting-clip-bones-poses", "[animation]") {
    using tests::MathUtils;

    Skeleton skeleton = generateTestSkeleton();
    SkeletalAnimationClip clip = generateTestAnimationClip();

    // Root bone poses
    BonePose rootBonePose = clip.getBoneRelativePose(0, 15.0f);

    REQUIRE(MathUtils::isEqual(rootBonePose.position, { 15.0f, 0.0f, 0.0f }));
    REQUIRE(MathUtils::isEqual(rootBonePose.orientation, MathUtils::IDENTITY_QUAT));

    // Spin bone poses
    BonePose spinBonePose = clip.getBoneRelativePose(1, 15.0f);

    REQUIRE(MathUtils::isEqual(spinBonePose.position, { 0.0f, 15.0f, 0.0f }));
    REQUIRE(MathUtils::isEqual(spinBonePose.orientation, glm::slerp(glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X),
                                         glm::angleAxis(glm::radians(90.0f), MathUtils::AXIS_X), 0.5f)));

    BonePose spinBonePoseLast = clip.getBoneRelativePose(1, 60.0f);

    REQUIRE(MathUtils::isEqual(spinBonePoseLast.orientation, glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X)));

    BonePose spinBonePoseTimeOverflow = clip.getBoneRelativePose(2, 123.0f);

    REQUIRE(MathUtils::isEqual(spinBonePoseTimeOverflow.position, spinBonePoseLast.position));
    REQUIRE(MathUtils::isEqual(spinBonePoseTimeOverflow.orientation, spinBonePoseLast.orientation));

    // Spin end bone poses
    BonePose spinEndBonePose = clip.getBoneRelativePose(2, 15.0f);

    REQUIRE(MathUtils::isEqual(spinEndBonePose.orientation, MathUtils::IDENTITY_QUAT));
}
