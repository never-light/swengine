#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>
#include <vector>

#include "AnimationClip.h"
#include "AnimationState.h"
#include "AnimationStatesMachineVariables.h"
#include "AnimationClipInstance.h"
#include "SkeletalAnimationPose.h"

class AnimationStatesMachine {
 public:
  explicit AnimationStatesMachine(std::shared_ptr<Skeleton> skeleton);

  [[nodiscard]] int16_t getStateIdByName(const std::string& name) const;

  [[nodiscard]] AnimationState& addState(const std::string& name,
    std::unique_ptr<AnimationPoseNode> initialPoseNode);

  void addTransition(int16_t sourceStateId, int16_t targetStateId);

  void switchToNextState(int16_t stateId);

  void setActiveState(int16_t stateId);
  [[nodiscard]] const AnimationState& getActiveState() const;
  [[nodiscard]] AnimationState& getActiveState();

  [[nodiscard]] const SkeletalAnimationPose& getCurrentPose() const;
  [[nodiscard]] const AnimationMatrixPalette& getCurrentMatrixPalette() const;

  [[nodiscard]] const AnimationStatesMachineVariables& getVariablesSet() const;
  [[nodiscard]] AnimationStatesMachineVariables& getVariablesSet();

  void increaseCurrentTime(float delta);

 private:
  std::shared_ptr<Skeleton> m_skeleton;

  std::unordered_map<std::string, int16_t> m_statesNameToIdMap;
  std::vector<AnimationState> m_states;

  AnimationStatesMachineVariables m_variablesSet;

  int16_t m_activeStateId = -1;
};
