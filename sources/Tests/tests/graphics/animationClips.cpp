#include "animation.h"

TEST_CASE("bone-pose-matrix", "[animation]")
{
  using tests::MathUtils;

  glm::vec3 originPosition = {1.0f, 2.0f, 3.0f};
  glm::quat originOrientation = glm::angleAxis(glm::radians(45.0f), MathUtils::AXIS_X);
  BonePose originPose(originPosition, originOrientation);

  REQUIRE(MathUtils::isEqual(originPose.getBoneMatrix(), MathUtils::getTranslationMatrix(originPosition) *
    glm::mat4_cast(originOrientation)));
}

TEST_CASE("bone-poses-interpolation", "[animation]")
{
  using tests::MathUtils;

  glm::vec3 originPosition = {1.0f, 2.0f, 3.0f};
  glm::quat originOrientation = glm::angleAxis(glm::radians(45.0f), MathUtils::AXIS_X);
  BonePose originPose(originPosition, originOrientation);

  SECTION("interpolation") {
    glm::vec3 targetPosition = {2.0f, 4.0f, 6.0f};
    glm::quat targetOrientation = glm::angleAxis(glm::radians(45.0f), MathUtils::AXIS_Y);

    BonePose targetPose(targetPosition, targetOrientation);

    float interpolationFactor = 0.5f;
    BonePose interpolatedPose = BonePose::interpolate(originPose, targetPose, interpolationFactor);

    glm::vec3 interpolatedPosition = glm::mix(originPosition, targetPosition, interpolationFactor);
    glm::quat interpolatedOrientation = glm::slerp(originOrientation, targetOrientation, interpolationFactor);

    REQUIRE(MathUtils::isEqual(interpolatedPose.position, interpolatedPosition));
    REQUIRE(MathUtils::isEqual(interpolatedPose.orientation, interpolatedOrientation));

    REQUIRE(MathUtils::isEqual(interpolatedPose.getBoneMatrix(),
      MathUtils::getTranslationMatrix(interpolatedPosition) *
        glm::mat4_cast(interpolatedOrientation)));
  }
}

TEST_CASE("getting-clip-bones-poses", "[animation]")
{
  using tests::MathUtils;

  Skeleton skeleton = generateTestSkeleton();
  AnimationClip clip = generateTestAnimationClip();

  // Root bone poses
  BonePose rootBonePose = clip.getBoneRelativePose(0, 15.0f);

  REQUIRE(MathUtils::isEqual(rootBonePose.position, {15.0f, 0.0f, 0.0f}));
  REQUIRE(MathUtils::isEqual(rootBonePose.orientation, MathUtils::IDENTITY_QUAT));

  // Spin bone poses
  BonePose spinBonePose = clip.getBoneRelativePose(1, 15.0f);

  REQUIRE(MathUtils::isEqual(spinBonePose.position, {0.0f, 15.0f, 0.0f}));
  REQUIRE(MathUtils::isEqual(spinBonePose.orientation, glm::slerp(glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X),
    glm::angleAxis(glm::radians(90.0f),
      MathUtils::AXIS_X), 0.5f)));

  BonePose spinBonePoseLast = clip.getBoneRelativePose(1, 60.0f);

  REQUIRE(MathUtils::isEqual(spinBonePoseLast.orientation, glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X)));

  BonePose spinBonePoseTimeOverflow = clip.getBoneRelativePose(2, 123.0f);

  REQUIRE(MathUtils::isEqual(spinBonePoseTimeOverflow.position, spinBonePoseLast.position));
  REQUIRE(MathUtils::isEqual(spinBonePoseTimeOverflow.orientation, spinBonePoseLast.orientation));

  // Spin end bone poses
  BonePose spinEndBonePose = clip.getBoneRelativePose(2, 15.0f);

  REQUIRE(MathUtils::isEqual(spinEndBonePose.orientation, MathUtils::IDENTITY_QUAT));
}

TEST_CASE("clip-instance-increasing-time", "[animation]")
{
  using tests::MathUtils;

  SkeletalAnimationClipInstance clipInstance = generateTestAnimationClipInstance();
  std::shared_ptr<Skeleton> skeleton = clipInstance.getSkeletonPtr();

  SECTION("clip-zero-time") {
    SkeletalAnimationPose pose = clipInstance.getAnimationPose();

    REQUIRE(MathUtils::isEqual(pose.getBoneLocalPose(0).getBoneMatrix(), MathUtils::IDENTITY_MATRIX4));
    REQUIRE(MathUtils::isEqual(pose.getBoneLocalPose(1).getBoneMatrix(), MathUtils::IDENTITY_MATRIX4));
    REQUIRE(MathUtils::isEqual(pose.getBoneLocalPose(2).getBoneMatrix(), MathUtils::IDENTITY_MATRIX4));

    AnimationMatrixPalette matrixPalette = pose.getMatrixPalette();

    REQUIRE(MathUtils::isEqual(matrixPalette.bonesTransforms[0],
      MathUtils::IDENTITY_MATRIX4 * skeleton->getBone(0).getInverseBindPoseMatrix()));

    REQUIRE(MathUtils::isEqual(matrixPalette.bonesTransforms[1],
      MathUtils::IDENTITY_MATRIX4 * skeleton->getBone(1).getInverseBindPoseMatrix()));

    REQUIRE(MathUtils::isEqual(matrixPalette.bonesTransforms[2],
      MathUtils::IDENTITY_MATRIX4 * skeleton->getBone(2).getInverseBindPoseMatrix()));
  }

  SECTION("clip-middle-time") {
    clipInstance.increaseCurrentTime(0.5f);

    SkeletalAnimationPose pose = clipInstance.getAnimationPose();
    AnimationMatrixPalette matrixPalette = pose.getMatrixPalette();

    // Root bone
    glm::mat4 rootBoneTranslation = MathUtils::getTranslationMatrix({15.0f, 0.0f, 0.0f});
    REQUIRE(MathUtils::isEqual(pose.getBoneLocalPose(0).getBoneMatrix(), rootBoneTranslation));

    REQUIRE(MathUtils::isEqual(matrixPalette.bonesTransforms[0],
      rootBoneTranslation * skeleton->getBone(0).getInverseBindPoseMatrix()));

    // Spin bone
    glm::mat4 spinBoneRotation = glm::mat4_cast(glm::slerp(glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X),
      glm::angleAxis(glm::radians(90.0f), MathUtils::AXIS_X),
      0.5f));

    glm::mat4 spinBoneTranslation = MathUtils::getTranslationMatrix({0.0f, 15.0f, 0.0f});

    REQUIRE(MathUtils::isEqual(pose.getBoneLocalPose(1).getBoneMatrix(),
      spinBoneTranslation * spinBoneRotation));

    REQUIRE(MathUtils::isEqual(matrixPalette.bonesTransforms[1],
      spinBoneTranslation * spinBoneRotation * rootBoneTranslation *
        skeleton->getBone(1).getInverseBindPoseMatrix()));
  }

  SECTION("clip-overflow-time") {
    clipInstance.setEndBehaviour(SkeletalAnimationClipEndBehaviour::Repeat);
    clipInstance.increaseCurrentTime(2.5f);

    SkeletalAnimationPose pose = clipInstance.getAnimationPose();
    AnimationMatrixPalette matrixPalette = pose.getMatrixPalette();

    // Root bone
    glm::mat4 rootBoneTranslation = MathUtils::getTranslationMatrix({15.0f, 0.0f, 0.0f});
    REQUIRE(MathUtils::isEqual(pose.getBoneLocalPose(0).getBoneMatrix(), rootBoneTranslation));

    REQUIRE(MathUtils::isEqual(matrixPalette.bonesTransforms[0],
      rootBoneTranslation * skeleton->getBone(0).getInverseBindPoseMatrix()));

    // Spin bone
    glm::mat4 spinBoneRotation = glm::mat4_cast(glm::slerp(glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X),
      glm::angleAxis(glm::radians(90.0f), MathUtils::AXIS_X),
      0.5f));

    glm::mat4 spinBoneTranslation = MathUtils::getTranslationMatrix({0.0f, 15.0f, 0.0f});

    REQUIRE(MathUtils::isEqual(pose.getBoneLocalPose(1).getBoneMatrix(),
      spinBoneTranslation * spinBoneRotation));

    REQUIRE(MathUtils::isEqual(matrixPalette.bonesTransforms[1],
      spinBoneTranslation * spinBoneRotation * rootBoneTranslation *
        skeleton->getBone(1).getInverseBindPoseMatrix()));
  }
}
