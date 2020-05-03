#include "animation.h"

#include <spdlog/spdlog.h>
#include <glm/gtx/string_cast.hpp>

#include <Engine/swdebug.h>

TEST_CASE("state-machine-variables-get-set", "[animation]")
{
  using tests::MathUtils;

  auto skeleton = std::make_shared<Skeleton>(generateTestSkeleton());
  AnimationStatesMachine stateMachine(skeleton);

  AnimationStatesMachineVariables& variablesSet = stateMachine.getVariablesSet();

  SkeletalAnimationVariableId speedVarId = variablesSet.registerVariable("player_speed", 0.0f);
  variablesSet.setVariableValue("player_speed", 15.0f);

  REQUIRE(variablesSet.getVariableId("player_speed") == speedVarId);
  REQUIRE(MathUtils::isEqual(variablesSet.getVariableValue("player_speed"), 15.0f));
  REQUIRE(MathUtils::isEqual(variablesSet.getVariableValue(speedVarId), 15.0f));
}

TEST_CASE("state-machine-states-clip-pose-node", "[animation]")
{
  using tests::MathUtils;

  SkeletalAnimationClipPoseNode clipPoseNode(generateTestAnimationClipInstance());
  AnimationStatesMachineVariables variablesSet;

  clipPoseNode.startAnimation();
  clipPoseNode.increaseCurrentTime(0.5f, variablesSet);

  AnimationPose animationPose = clipPoseNode.getCurrentPose();

  // Root bone
  REQUIRE(MathUtils::isEqual(animationPose.getBoneLocalPose(0).getBoneMatrix(),
    MathUtils::getTranslationMatrix({15.0f, 0.0f, 0.0f})));

  // Spin bone
  glm::mat4 spinBoneRotation = glm::mat4_cast(glm::slerp(glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X),
    glm::angleAxis(glm::radians(90.0f), MathUtils::AXIS_X), 0.5f));

  glm::mat4 spinBoneTranslation = MathUtils::getTranslationMatrix({0.0f, 15.0f, 0.0f});

  REQUIRE(MathUtils::isEqual(animationPose.getBoneLocalPose(1).getBoneMatrix(),
    spinBoneTranslation * spinBoneRotation));

}

TEST_CASE("state-machine-states-blend-pose-node", "[animation]")
{
  using tests::MathUtils;

  std::shared_ptr<Skeleton> skeleton = std::make_shared<Skeleton>(
    std::vector<Bone>({Bone("root", Bone::ROOT_BONE_PARENT_ID, MathUtils::IDENTITY_MATRIX4)}));

  std::shared_ptr<AnimationClip> firstAnimationClip =
    std::make_shared<AnimationClip>("first", 60.0f, 30.0f, std::vector<BoneAnimationChannel>{
      BoneAnimationChannel(BoneAnimationChannel({
        BoneAnimationPositionFrame{0.0f, {0.0f, 0.0f, 0.0f}},
        BoneAnimationPositionFrame{30.0f, {30.0f, 0.0f, 0.0f}},
        BoneAnimationPositionFrame{60.0f, {0.0f, 0.0f, 0.0f}}
      }, {}))});

  AnimationClipInstance firstAnimationClipInstance(skeleton, firstAnimationClip);

  std::shared_ptr<AnimationClip> secondAnimationClip =
    std::make_shared<AnimationClip>("second", 60.0f, 30.0f, std::vector<BoneAnimationChannel>{
      BoneAnimationChannel(BoneAnimationChannel({
        BoneAnimationPositionFrame{0.0f, {0.0f, 0.0f, 0.0f}},
        BoneAnimationPositionFrame{30.0f, {0.0f, 30.0f, 0.0f}},
        BoneAnimationPositionFrame{60.0f, {0.0f, 0.0f, 0.0f}}
      }, {}))});

  AnimationClipInstance secondAnimationClipInstance(skeleton, secondAnimationClip);

  AnimationStatesMachineVariables variablesSet;
  SkeletalAnimationVariableId blendFactorVariableId = variablesSet.registerVariable("blend_factor", 0.0f);

  SECTION("linear-blending") {
    AnimationBlendPoseNode poseNode(std::make_shared<SkeletalAnimationClipPoseNode>(firstAnimationClipInstance),
    std::make_shared<SkeletalAnimationClipPoseNode>(secondAnimationClipInstance),
      blendFactorVariableId,
      SkeletalAnimationBlendPoseType::Linear, 0);

    variablesSet.setVariableValue(blendFactorVariableId, 0.5f);

    poseNode.startAnimation();
    poseNode.increaseCurrentTime(0.5f, variablesSet);

    AnimationPose animationPose = poseNode.getCurrentPose();

    glm::vec3 targetBonePose = (firstAnimationClip->getBoneRelativePose(0, 15.0f).position +
      secondAnimationClip->getBoneRelativePose(0, 15.0f).position) / 2.0f;

    REQUIRE(MathUtils::isEqual(animationPose.getBoneLocalPose(0).position, targetBonePose));
  }

  SECTION("override-blending") {
    AnimationBlendPoseNode poseNode(std::make_shared<SkeletalAnimationClipPoseNode>(firstAnimationClipInstance),
      std::make_shared<SkeletalAnimationClipPoseNode>(secondAnimationClipInstance),
      blendFactorVariableId,
      SkeletalAnimationBlendPoseType::Override, 0);

    variablesSet.setVariableValue(blendFactorVariableId, 0.5f);

    poseNode.startAnimation();
    poseNode.increaseCurrentTime(0.5f, variablesSet);

    AnimationPose animationPose = poseNode.getCurrentPose();

    glm::vec3 targetBonePose = secondAnimationClip->getBoneRelativePose(0, 15.0f).position;

    REQUIRE(MathUtils::isEqual(animationPose.getBoneLocalPose(0).position, targetBonePose));
  }

}
