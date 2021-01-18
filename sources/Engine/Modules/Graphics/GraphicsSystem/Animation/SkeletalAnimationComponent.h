#pragma once

#include <memory>

#include "AnimationStatesMachine.h"
#include "Modules/ECS/GameObjectsFactory.h"

class AnimationComponentBindingParameters {
 public:
  std::string skeletonResourceName;
  std::string stateMachineResourceName;
  std::string stateMachineInitialState;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("skeleton_resource", skeletonResourceName),
      cereal::make_nvp("state_machine_resource", stateMachineResourceName),
      cereal::make_nvp("state_machine_initial_state", stateMachineInitialState));
  };
};

class SkeletalAnimationComponent {
 public:
  static constexpr bool s_isSerializable = true;
  using BindingParameters = AnimationComponentBindingParameters;

 public:
  explicit SkeletalAnimationComponent(ResourceHandle<AnimationStatesMachine> animationStatesMachine);

  void setAnimationStatesMachine(ResourceHandle<AnimationStatesMachine> statesMachine);
  [[nodiscard]] ResourceHandle<AnimationStatesMachine> getAnimationStatesMachine() const;

  AnimationStatesMachine& getAnimationStatesMachineRef();
  [[nodiscard]] const AnimationStatesMachine& getAnimationStatesMachineRef() const;

  [[nodiscard]] const AnimationMatrixPalette& getMatrixPalette() const;

  [[nodiscard]] BindingParameters getBindingParameters() const;

 private:
  ResourceHandle<AnimationStatesMachine> m_animationStatesMachine;
};

class AnimationComponentBinder : public GameObjectsComponentBinder<SkeletalAnimationComponent> {
 public:
  explicit AnimationComponentBinder(const ComponentBindingParameters& componentParameters,
    std::shared_ptr<ResourcesManager> resourcesManager);

  void bindToObject(GameObject& gameObject) override;

 private:
  ComponentBindingParameters m_bindingParameters;
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};
