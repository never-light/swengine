#include "precompiled.h"

#pragma hdrstop

#include "AnimationStatesMachineResource.h"
#include "Exceptions/exceptions.h"

#include <fstream>
#include <streambuf>
#include <bitset>
#include <algorithm>

#include "Modules/Graphics/OpenGL/GLShadersPipeline.h"
#include "Modules/ResourceManagement/ResourceManager.h"
#include "SkeletalAnimationResource.h"
#include "SkeletonResource.h"

AnimationStatesMachineResource::AnimationStatesMachineResource(ResourcesManager* resourcesManager)
  : ResourceTypeManager<AnimationStatesMachine, AnimationStatesMachineParameters>(resourcesManager)
{

}

AnimationStatesMachineResource::~AnimationStatesMachineResource() = default;


void AnimationStatesMachineResource::load(size_t resourceIndex)
{
  AnimationStatesMachineParameters* config = getResourceConfig(resourceIndex);

  ResourceHandle<Skeleton> skeleton =
    getResourceManager()->getResource<Skeleton>(config->skeletonName);

  auto* statesMachine = allocateResource<AnimationStatesMachine>(resourceIndex, skeleton);

  auto& animationVariablesSet = statesMachine->getVariablesSet();

  for (const auto& variableDefinition : config->variables) {
    RETURN_VALUE_UNUSED(animationVariablesSet.registerVariable(variableDefinition.name, variableDefinition.value));
  }

  for (const auto& stateParameters : config->states) {
    statesMachine->addState(stateParameters.name,
      stateParameters.animationsTreeRoot->getPoseNode(skeleton,
        animationVariablesSet, *getResourceManager()));
  }

  for (const auto& stateParameters : config->states) {
    auto& state = statesMachine->getState(stateParameters.name);

    // Final action
    state.setFinalAction(stateParameters.finalAction.type);

    switch (stateParameters.finalAction.type) {
      case AnimationFinalAction::SwitchState:
        state.setFinalTransitionStateId(statesMachine->getStateIdByName(stateParameters.finalAction.nextStateName));
        break;

      case AnimationFinalAction::Repeat:
      case AnimationFinalAction::Stop:
        break;

      default:
        THROW_EXCEPTION(NotImplementedException, "Animation final action is not supported: " +
          std::to_string(static_cast<int>(stateParameters.finalAction.type)));
    }

    // Transitions

    for (const auto& transitionParameters : stateParameters.transitions) {
      AnimationTransition transition(transitionParameters.type, transitionParameters.duration);

      switch (transitionParameters.type) {
        case AnimationStatesTransitionType::SmoothLinear:
          break;

        case AnimationStatesTransitionType::Straight:
          break;

        default:
          THROW_EXCEPTION(NotImplementedException, "Transition type is not supported: " +
            std::to_string(static_cast<int>(transitionParameters.type)));
      }

      statesMachine->setTransition(statesMachine->getStateIdByName(stateParameters.name),
        statesMachine->getStateIdByName(transitionParameters.targetStateName),
        transition);
    }
  }
}

void AnimationStatesMachineResource::parseConfig(size_t resourceIndex, pugi::xml_node configNode)
{
  using MachineParams = AnimationStatesMachineParameters;

  static std::unordered_map<std::string, AnimationFinalAction> finalActionTypesMap = {
    {"repeat", AnimationFinalAction::Repeat},
    {"stop", AnimationFinalAction::Stop},
    {"switch_state", AnimationFinalAction::SwitchState},
  };

  static std::unordered_map<std::string, AnimationStatesTransitionType> transitionTypesMap = {
    {"straight", AnimationStatesTransitionType::Straight},
    {"smooth_linear", AnimationStatesTransitionType::SmoothLinear},
  };

  MachineParams* resourceConfig = createResourceConfig(resourceIndex);

  // Skeleton
  resourceConfig->skeletonName = configNode.child("skeleton").attribute("id").as_string();

  // Variables
  pugi::xml_node variablesNode = configNode.child("variables");

  for (auto variableNode : variablesNode.children()) {
    std::string name = variableNode.attribute("name").as_string();
    float value = variableNode.attribute("value").as_float();

    resourceConfig->variables.emplace_back(name, value);
  }

  // States

  pugi::xml_node statesNode = configNode.child("states");

  for (auto stateNode : statesNode.children()) {
    MachineParams::State state;

    state.name = stateNode.attribute("name").as_string();

    // Animation tree
    state.animationsTreeRoot = createAnimationNode(stateNode.child("anim").child("node"));

    // Final action
    pugi::xml_node finalActionNode = stateNode.child("final_action");

    std::string finalActionType = StringUtils::toLowerCase(finalActionNode.attribute("type").as_string());

    state.finalAction.type = ResourceDeclHelpers::getFilteredParameterValue(finalActionType,
      "final_action_type",
      finalActionTypesMap,
      AnimationFinalAction::Stop);

    switch (state.finalAction.type) {
      case AnimationFinalAction::SwitchState:
        state.finalAction.nextStateName = finalActionNode.attribute("target").as_string();
        break;

      case AnimationFinalAction::Stop:
      case AnimationFinalAction::Repeat:
        break;

      default:
        THROW_EXCEPTION(NotImplementedException, "Animation final action is not supported: " +
          std::to_string(static_cast<int>(state.finalAction.type)));
    }

    // Transitions
    pugi::xml_node transitionsNode = stateNode.child("transitions");

    for (auto transitionNode : transitionsNode.children()) {
      MachineParams::Transition transition;

      std::string transitionType = StringUtils::toLowerCase(transitionNode.attribute("type").as_string());

      transition.type = ResourceDeclHelpers::getFilteredParameterValue(transitionType,
        "transition_type",
        transitionTypesMap,
        AnimationStatesTransitionType::Straight);

      switch (transition.type) {
        case AnimationStatesTransitionType::SmoothLinear:
        case AnimationStatesTransitionType::Straight:
          transition.duration = transitionNode.attribute("duration").as_float();
          transition.targetStateName = transitionNode.attribute("target").as_string();
          break;

        default:
          THROW_EXCEPTION(NotImplementedException, "Transition type is not supported: " +
            std::to_string(static_cast<int>(transition.type)));
      }

      state.transitions.push_back(std::move(transition));
    }

    // States compound
    resourceConfig->states.push_back(std::move(state));
  }
}


std::shared_ptr<AnimationStatesMachineParameters::Node> AnimationStatesMachineResource::createAnimationNode(
  const pugi::xml_node& node)
{
  std::string nodeType = node.attribute("type").as_string();

  if (nodeType == "clip") {
    pugi::xml_node clipNode = node.child("clip");

    std::string clipName = clipNode.attribute("id").as_string();
    float clipScale = clipNode.attribute("scale").as_float();

    return std::make_shared<AnimationStatesMachineParameters::ClipNode>(
      AnimationStatesMachineParameters::Clip(clipName, clipScale));
  }
  else if (nodeType == "linear_blend" || nodeType == "additive_blend") {
    auto firstNode = createAnimationNode(node.child("first").child("node"));
    auto secondNode = createAnimationNode(node.child("second").child("node"));
    std::string blendFactorVariableName = node.child("blend_factor_variable").attribute("name").as_string();

    SkeletalAnimationBlendPoseType blendType = SkeletalAnimationBlendPoseType::Additive;

    if (nodeType == "linear_blend") {
      blendType = SkeletalAnimationBlendPoseType::Linear;
    }
    else if (nodeType == "additive_blend") {
      blendType = SkeletalAnimationBlendPoseType::Additive;
    }
    else {
      THROW_EXCEPTION(NotImplementedException, "Blend type is not supported: " + nodeType);
    }

    return std::make_shared<AnimationStatesMachineParameters::BlendClipsNode>(
      std::move(firstNode), std::move(secondNode), blendFactorVariableName, blendType);
  }
  else {
    THROW_EXCEPTION(EngineRuntimeException, "Invalid animation node type: " + nodeType);
  }
}

std::shared_ptr<AnimationPoseNode> AnimationStatesMachineParameters::BlendClipsNode::getPoseNode(
  ResourceHandle<Skeleton> skeleton,
  AnimationStatesMachineVariables& variablesSet,
  ResourcesManager& resourceManager) const
{
  return std::make_shared<AnimationBlendPoseNode>(first->getPoseNode(skeleton, variablesSet, resourceManager),
    second->getPoseNode(skeleton, variablesSet, resourceManager),
    variablesSet.getVariableId(blendFactorVariableName),
    blendType);
}

std::shared_ptr<AnimationPoseNode> AnimationStatesMachineParameters::ClipNode::getPoseNode(
  ResourceHandle<Skeleton> skeleton,
  AnimationStatesMachineVariables& variablesSet,
  ResourcesManager& resourceManager) const
{
  ARG_UNUSED(variablesSet);

  ResourceHandle<AnimationClip> animationClipResource = resourceManager.getResource<AnimationClip>(clip.name);

  AnimationClipInstance clipInstance(skeleton, animationClipResource);
  clipInstance.setScale(clip.scale);

  return std::make_shared<SkeletalAnimationClipPoseNode>(clipInstance);
}
