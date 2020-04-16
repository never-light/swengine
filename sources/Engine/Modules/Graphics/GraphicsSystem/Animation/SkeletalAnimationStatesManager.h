#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <string>
#include <vector>

#include "SkeletalAnimationClip.h"
#include "SkeletalAnimationState.h"
#include "SkeletalAnimationStatesMachineVariables.h"

class SkeletalAnimationStatesMachine {
 public:
  explicit SkeletalAnimationStatesMachine(std::shared_ptr<Skeleton> skeleton);

  [[nodiscard]] int16_t getStateIdByName(const std::string& name) const;

  [[nodiscard]] SkeletalAnimationState& addState(const std::string& name,
      std::unique_ptr<SkeletalAnimationPoseNode> initialPoseNode);

  void addTransition(int16_t sourceStateId, int16_t targetStateId);

  void switchToNextState(int16_t stateId);

  void setActiveState(int16_t stateId);
  [[nodiscard]] const SkeletalAnimationState& getActiveState() const;
  [[nodiscard]] SkeletalAnimationState& getActiveState();

  [[nodiscard]] const SkeletalAnimationPose& getCurrentPose() const;
  [[nodiscard]] const SkeletalAnimationMatrixPalette& getCurrentMatrixPalette() const;

  [[nodiscard]] const SkeletalAnimationStatesMachineVariables& getVariablesSet() const;
  [[nodiscard]] SkeletalAnimationStatesMachineVariables& getVariablesSet();

  void increaseCurrentTime(float delta);

 private:
  std::shared_ptr<Skeleton> m_skeleton;

  std::unordered_map<std::string, int16_t> m_statesNameToIdMap;
  std::vector<SkeletalAnimationState> m_states;

  SkeletalAnimationStatesMachineVariables m_variablesSet;

  int16_t m_activeStateId = -1;
};
