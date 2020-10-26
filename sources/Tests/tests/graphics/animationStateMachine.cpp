#include "animation.h"

#include <glm/gtx/string_cast.hpp>

TEST_CASE("state_machine_variables_get_set", "[graphics]animation]")
{
  std::shared_ptr<ResourcesManager> resourcesManager = generateTestResourcesManager();

  auto skeleton = resourcesManager->createResourceInPlace<Skeleton>(generateTestSkeleton());
  AnimationStatesMachine stateMachine(skeleton);

  AnimationStatesMachineVariables& variablesSet = stateMachine.getVariablesSet();

  SkeletalAnimationVariableId speedVarId = variablesSet.registerVariable("player_speed", 0.0f);
  variablesSet.setVariableValue("player_speed", 15.0f);

  REQUIRE(variablesSet.getVariableId("player_speed") == speedVarId);
  REQUIRE(MathUtils::isEqual(variablesSet.getVariableValue("player_speed"), 15.0f));
  REQUIRE(MathUtils::isEqual(variablesSet.getVariableValue(speedVarId), 15.0f));
}

TEST_CASE("state_machine_states_clip_pose_node", "[graphics][animation]")
{
  std::shared_ptr<ResourcesManager> resourcesManager = generateTestResourcesManager();

  SkeletalAnimationClipPoseNode clipPoseNode(generateTestAnimationClipInstance(*resourcesManager));
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

TEST_CASE("state_machine_states_blend_pose_node", "[graphics][animation]")
{
  std::shared_ptr<ResourcesManager> resourcesManager = generateTestResourcesManager();

  ResourceHandle<Skeleton> skeleton = resourcesManager->createResourceInPlace<Skeleton>(
    std::vector<Bone>({Bone("root", Bone::ROOT_BONE_PARENT_ID, MathUtils::IDENTITY_MATRIX4)}));

  ResourceHandle<AnimationClip> firstAnimationClip =
    resourcesManager->createResourceInPlace<AnimationClip>("first", 60.0f, 30.0f, std::vector<BoneAnimationChannel>{
      BoneAnimationChannel({
        BoneAnimationPositionFrame{0.0f, {0.0f, 0.0f, 0.0f}},
        BoneAnimationPositionFrame{30.0f, {30.0f, 0.0f, 0.0f}},
        BoneAnimationPositionFrame{60.0f, {0.0f, 0.0f, 0.0f}}
      }, {})});

  AnimationClipInstance firstAnimationClipInstance(skeleton, firstAnimationClip);

  ResourceHandle<AnimationClip> secondAnimationClip =
    resourcesManager->createResourceInPlace<AnimationClip>("second", 60.0f, 30.0f, std::vector<BoneAnimationChannel>{
      BoneAnimationChannel({
        BoneAnimationPositionFrame{0.0f, {0.0f, 0.0f, 0.0f}},
        BoneAnimationPositionFrame{30.0f, {0.0f, 30.0f, 0.0f}},
        BoneAnimationPositionFrame{60.0f, {0.0f, 0.0f, 0.0f}}
      }, {})});

  AnimationClipInstance secondAnimationClipInstance(skeleton, secondAnimationClip);

  AnimationStatesMachineVariables variablesSet;
  SkeletalAnimationVariableId blendFactorVariableId = variablesSet.registerVariable("blend_factor", 0.0f);

  SECTION("linear_blending") {
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

  SECTION("override_blending") {
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

  SECTION("additive_blending") {
    ResourceHandle<AnimationClip> mainAnimationClip =
      resourcesManager->createResourceInPlace<AnimationClip>("first", 60.0f, 30.0f, std::vector<BoneAnimationChannel>{
        BoneAnimationChannel({
          BoneAnimationPositionFrame{0.0f, {0.0f, 0.0f, 0.0f}},
          BoneAnimationPositionFrame{60.0f, {0.0f, 0.0f, 0.0f}}
        }, {
          BoneAnimationOrientationFrame{0.0f, glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_Y)},
          BoneAnimationOrientationFrame{0.0f, glm::angleAxis(glm::radians(30.0f), MathUtils::AXIS_Y)},
        })});

    AnimationClipInstance mainAnimationClipInstance(skeleton, mainAnimationClip);

    ResourceHandle<AnimationClip> additiveAnimationClip =
      resourcesManager->createResourceInPlace<AnimationClip>("second", 60.0f, 30.0f, std::vector<BoneAnimationChannel>{
        BoneAnimationChannel({
          BoneAnimationPositionFrame{0.0f, {0.0f, 0.0f, 0.0f}},
          BoneAnimationPositionFrame{30.0f, {10.0f, 20.0f, 30.0f}},
          BoneAnimationPositionFrame{60.0f, {20.0f, 40.0f, 60.0f}}
        }, {
          BoneAnimationOrientationFrame{0.0f, glm::angleAxis(glm::radians(0.0f), MathUtils::AXIS_X)},
          BoneAnimationOrientationFrame{0.0f, glm::angleAxis(glm::radians(30.0f), MathUtils::AXIS_X)},
        })});

    AnimationClipInstance additiveAnimationClipInstance(skeleton, additiveAnimationClip);

    AnimationBlendPoseNode poseNode(std::make_shared<SkeletalAnimationClipPoseNode>(mainAnimationClipInstance),
      std::make_shared<SkeletalAnimationClipPoseNode>(additiveAnimationClipInstance),
      blendFactorVariableId,
      SkeletalAnimationBlendPoseType::Additive, 0);

    variablesSet.setVariableValue(blendFactorVariableId, 0.5f);

    poseNode.startAnimation();
    poseNode.increaseCurrentTime(2.0f, variablesSet);

    AnimationPose animationPose = poseNode.getCurrentPose();

    glm::vec3 targetBonePose = mainAnimationClip->getBoneRelativePose(0, 60.0f).position +
      additiveAnimationClip->getBoneRelativePose(0, 60.0f).position * 0.5f;

    glm::quat targetBoneOrientation = glm::slerp(
      mainAnimationClip->getBoneRelativePose(0, 60.0f).orientation,
      additiveAnimationClip->getBoneRelativePose(0, 60.0f).orientation *
        mainAnimationClip->getBoneRelativePose(0, 60.0f).orientation,
      0.5f);

    auto pts = animationPose.getBoneLocalPose(0).position;

    REQUIRE(MathUtils::isEqual(animationPose.getBoneLocalPose(0).position, targetBonePose));
    REQUIRE(MathUtils::isEqual(animationPose.getBoneLocalPose(0).orientation, targetBoneOrientation));
  }

}

TEST_CASE("state_machine_conditional_transitions", "[graphics][animation]")
{
  std::shared_ptr<ResourcesManager> resourcesManager = generateTestResourcesManager();

  auto clipInstance = generateTestAnimationClipInstance(*resourcesManager);

  AnimationStatesMachine statesMachine(clipInstance.getSkeletonPtr());

  auto switchVariableId = statesMachine.getVariablesSet().registerVariable("need_to_switch", 0.0f);

  statesMachine.addState("first", std::make_shared<SkeletalAnimationClipPoseNode>(clipInstance));
  auto firstStateId = statesMachine.getStateIdByName("first");

  statesMachine.addState("second", std::make_shared<SkeletalAnimationClipPoseNode>(clipInstance));
  auto secondStateId = statesMachine.getStateIdByName("second");

  AnimationTransition transition = AnimationTransition(AnimationStatesTransitionType::Straight);
  transition.setCondition(std::make_shared<ComparisonNode>(
    std::make_unique<VariableReferenceNode>(switchVariableId),
    std::make_unique<ConstantNode>(1.0f),
    ComparisonOperation::Equal));

  statesMachine.setTransition(firstStateId,secondStateId, transition);

  statesMachine.setActiveState(firstStateId);

  REQUIRE(statesMachine.getActiveStateId() == firstStateId);

  statesMachine.increaseCurrentTime(0.1f);
  REQUIRE(statesMachine.getActiveStateId() == firstStateId);

  statesMachine.getVariablesSet().setVariableValue(switchVariableId, 0.5f);
  statesMachine.increaseCurrentTime(0.1f);
  REQUIRE(statesMachine.getActiveStateId() == firstStateId);

  statesMachine.getVariablesSet().setVariableValue(switchVariableId, 1.0f);
  statesMachine.increaseCurrentTime(0.1f);
  REQUIRE(statesMachine.getActiveStateId() == secondStateId);
}
