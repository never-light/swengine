#pragma once

#include <string>
#include <unordered_set>
#include <vector>
#include <memory>

#include "AnimationClip.h"
#include "AnimationStatesMachineVariables.h"
#include "AnimationClipInstance.h"
#include "AnimationPose.h"

#include "Nodes/AnimationClipPoseNode.h"
#include "Nodes/AnimationClipPoseNode.h"
#include "Nodes/AnimationBlendPoseNode.h"

enum class AnimationFinalAction {
  Repeat, Stop, SwitchState
};

class AnimationState {
 public:
  AnimationState(const std::string& name, std::shared_ptr<AnimationPoseNode> initialPoseNode);

  [[nodiscard]] const std::string& getName() const;

  void setFinalAction(AnimationFinalAction action);
  [[nodiscard]] AnimationFinalAction getFinalAction() const;

  void setFinalTransitionStateId(int16_t stateId);
  [[nodiscard]] int16_t getFinalTransitionStateId() const;

  void increaseCurrentTime(float delta, const AnimationStatesMachineVariables& variablesSet);

  [[nodiscard]] const AnimationPose& getCurrentPose() const;

  [[nodiscard]] const AnimationPoseNode& getInitialPoseNode() const;

  [[nodiscard]] float getCurrentTime() const;

 private:
  void activate();
  void deactivate();

 private:
  int16_t m_id;
  std::string m_name;

  AnimationFinalAction m_finalAction = AnimationFinalAction::Stop;
  int16_t m_finalTransitionStateId;

  std::shared_ptr<AnimationPoseNode> m_initialPoseNode;

  float m_currentTime = 0.0f;

 private:
  friend class AnimationStatesMachine;
};
