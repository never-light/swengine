#pragma once

#include <string>
#include <unordered_set>
#include <vector>
#include <memory>

#include "AnimationClip.h"
#include "AnimationStatesMachineVariables.h"
#include "AnimationClipInstance.h"
#include "SkeletalAnimationPose.h"

#include "Nodes/AnimationClipPoseNode.h"
#include "Nodes/AnimationClipPoseNode.h"
#include "Nodes/AnimationBlendPoseNode.h"

enum class SkeletalAnimationFinalAction {
  Repeat, Stop, SwitchState
};

class AnimationState {
 public:
  AnimationState(const std::string& name, std::unique_ptr<AnimationPoseNode> initialPoseNode);

  [[nodiscard]] const std::string& getName() const;

  void setFinalAction(SkeletalAnimationFinalAction action);
  [[nodiscard]] SkeletalAnimationFinalAction getFinalAction() const;

  void setFinalTransitionStateId(int16_t stateId);
  [[nodiscard]] int16_t getFinalTransitionStateId() const;

  [[nodiscard]] bool hasTransition(int16_t nextStateId) const;

  void increaseCurrentTime(float delta, const AnimationStatesMachineVariables& variablesSet);
  void resetCurrentTime();

  [[nodiscard]] const SkeletalAnimationPose& getCurrentPose() const;

 private:
  int16_t m_id;
  std::string m_name;

  std::unordered_set<int16_t> m_nextTransitions;

  SkeletalAnimationFinalAction m_finalAction = SkeletalAnimationFinalAction::Stop;
  int16_t m_finalTransitionStateId;

  std::unique_ptr<AnimationPoseNode> m_initialPoseNode;

 private:
  friend class AnimationStatesMachine;
};
