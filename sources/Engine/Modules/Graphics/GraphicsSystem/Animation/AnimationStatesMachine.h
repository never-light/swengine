#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>
#include <vector>

#include "Modules/ResourceManagement/ResourcesManagement.h"

#include "AnimationClip.h"
#include "AnimationState.h"
#include "AnimationStatesMachineVariables.h"
#include "AnimationClipInstance.h"
#include "AnimationPose.h"
#include "AnimationTransition.h"

class AnimationStatesMachine : public Resource {
 public:
  explicit AnimationStatesMachine(ResourceHandle<Skeleton> skeleton);
  ~AnimationStatesMachine() override = default;

  [[nodiscard]] int16_t getStateIdByName(const std::string& name) const;

  [[nodiscard]] AnimationState& getState(int16_t stateId);
  [[nodiscard]] const AnimationState& getState(int16_t stateId) const;

  [[nodiscard]] AnimationState& getState(const std::string& name);
  [[nodiscard]] const AnimationState& getState(const std::string& name) const;

  void addState(const std::string& name,
    std::shared_ptr<AnimationPoseNode> initialPoseNode);

  void setTransition(int16_t sourceStateId, int16_t targetStateId, AnimationTransition transition);
  [[nodiscard]] bool hasActiveTransition(int16_t sourceStateId, int16_t targetStateId) const;

  void switchToNextState(int16_t stateId);

  void setActiveState(int16_t stateId);
  void setActiveState(const std::string& stateName);

  [[nodiscard]] const AnimationState& getActiveState() const;
  [[nodiscard]] AnimationState& getActiveState();

  [[nodiscard]] int16_t getActiveStateId() const;
  [[nodiscard]] bool isActive() const;

  [[nodiscard]] const AnimationPose& getCurrentPose() const;
  [[nodiscard]] const AnimationMatrixPalette& getCurrentMatrixPalette() const;

  [[nodiscard]] const AnimationStatesMachineVariables& getVariablesSet() const;
  [[nodiscard]] AnimationStatesMachineVariables& getVariablesSet();

  [[nodiscard]] ResourceHandle<Skeleton> getSkeleton() const;

  void increaseCurrentTime(float delta);

 private:
  bool isTransitionActive() const;
  void finishActiveTransition();

 private:
  static constexpr int16_t INVALID_STATE_ID = -1;

 private:
  ResourceHandle<Skeleton> m_skeleton;

  std::unordered_map<std::string, int16_t> m_statesNameToIdMap;
  std::vector<AnimationState> m_states;
  std::vector<std::vector<AnimationTransition>> m_transitionsTable;

  AnimationStatesMachineVariables m_variablesSet;

  int16_t m_activeStateId = INVALID_STATE_ID;

  AnimationPose m_fadingPose;
  mutable AnimationPose m_smoothedPose;
  AnimationTransition* m_activeTransition = nullptr;
};
