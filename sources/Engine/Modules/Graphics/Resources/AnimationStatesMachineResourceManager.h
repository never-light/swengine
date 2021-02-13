#pragma once

#include <string>
#include <memory>
#include <utility>
#include <vector>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/GraphicsSystem/Animation/AnimationStatesMachine.h"

struct AnimationStatesMachineConfig {
  struct Variable {
    Variable(std::string name, float value) : name(std::move(name)), value(value)
    {};

    std::string name;
    float value = 0.0f;
  };

  struct Clip {
    Clip(std::string name, float scale) : name(std::move(name)), scale(scale)
    {};

    std::string name;
    float scale = 1.0f;
  };

  struct Node {
    Node() = default;
    virtual ~Node() = default;

    virtual std::shared_ptr<AnimationPoseNode> getPoseNode(
      ResourceHandle<Skeleton> skeleton,
      AnimationStatesMachineVariables& variablesSet,
      ResourcesManager& resourceManager) const = 0;
  };

  struct ClipNode : Node {
    explicit ClipNode(Clip clip) : Node(), clip(std::move(clip))
    {};
    ~ClipNode() override = default;

    std::shared_ptr<AnimationPoseNode> getPoseNode(
      ResourceHandle<Skeleton> skeleton,
      AnimationStatesMachineVariables& variablesSet,
      ResourcesManager& resourceManager) const override;

    Clip clip;
  };

  struct BlendClipsNode : Node {
    BlendClipsNode(std::shared_ptr<Node> first,
      std::shared_ptr<Node> second,
      std::string blendFactorVariableName,
      SkeletalAnimationBlendPoseType blendType) : Node(), first(std::move(first)),
                                                  second(std::move(second)),
                                                  blendFactorVariableName(std::move(blendFactorVariableName)),
                                                  blendType(blendType)
    {};

    ~BlendClipsNode() override = default;

    std::shared_ptr<AnimationPoseNode> getPoseNode(
      ResourceHandle<Skeleton> skeleton,
      AnimationStatesMachineVariables& variablesSet,
      ResourcesManager& resourceManager) const override;

    std::shared_ptr<Node> first;
    std::shared_ptr<Node> second;
    std::string blendFactorVariableName;
    SkeletalAnimationBlendPoseType blendType;

  };

  struct FinalAction {
    AnimationFinalAction type;
    std::string nextStateName;
  };

  struct Transition {
    std::string targetStateName;
    AnimationStatesTransitionType type;
    float duration = 0.0f;
  };

  struct State {
    std::string name;
    std::shared_ptr<Node> animationsTreeRoot;
    FinalAction finalAction;
    std::vector<Transition> transitions;
  };

  std::string skeletonName;
  std::vector<Variable> variables;
  std::vector<State> states;
};

class AnimationStatesMachineResourceManager
  : public ResourceManager<AnimationStatesMachine, AnimationStatesMachineConfig> {
 public:
  explicit AnimationStatesMachineResourceManager(ResourcesManager* resourcesManager);
  ~AnimationStatesMachineResourceManager() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;

 private:
  static std::shared_ptr<AnimationStatesMachineConfig::Node> createAnimationNode(const pugi::xml_node& node);
};
