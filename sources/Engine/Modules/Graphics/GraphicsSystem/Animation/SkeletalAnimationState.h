#pragma once

#include <string>
#include <unordered_set>
#include <vector>
#include <memory>

#include "SkeletalAnimationClip.h"
#include "SkeletalAnimationStatesMachineVariables.h"

class SkeletalAnimationStatesMachine;

enum class SkeletalAnimationFinalAction {
  Repeat, Stop, SwitchState
};

class SkeletalAnimationPoseNode {
 public:
  SkeletalAnimationPoseNode() = default;
  virtual ~SkeletalAnimationPoseNode() = default;

  virtual void increaseCurrentTime(float delta,
                                   const SkeletalAnimationStatesMachineVariables& variablesSet) = 0;

  [[nodiscard]] virtual const SkeletalAnimationPose& getCurrentPose() const = 0;
};

class SkeletalAnimationClipPoseNode : public SkeletalAnimationPoseNode {
 public:
  explicit SkeletalAnimationClipPoseNode(const SkeletalAnimationClipInstance& clip);
  ~SkeletalAnimationClipPoseNode() override = default;

  [[nodiscard]] const SkeletalAnimationPose& getCurrentPose() const override;

  void increaseCurrentTime(float delta,
                           const SkeletalAnimationStatesMachineVariables& variablesSet) override;

 private:
  SkeletalAnimationClipInstance m_clip;
};

enum class SkeletalAnimationBlendPoseType {
  Linear, Override
};

class SkeletalAnimationBlendPoseNode : public SkeletalAnimationPoseNode {
 public:
  SkeletalAnimationBlendPoseNode(const SkeletalAnimationClipInstance& firstClip,
                                 const SkeletalAnimationClipInstance& secondClip,
                                 SkeletalAnimationVariableId blendParameterVariableId,
                                 SkeletalAnimationBlendPoseType blendType = SkeletalAnimationBlendPoseType::Linear,
                                 uint8_t overriddenBone = 0);

  ~SkeletalAnimationBlendPoseNode() override;

  const SkeletalAnimationPose& getCurrentPose() const override;

  void increaseCurrentTime(float delta,
                           const SkeletalAnimationStatesMachineVariables& variablesSet) override;

  void setBlendParameterVariableId(SkeletalAnimationVariableId variableId);
  SkeletalAnimationVariableId getBlendParameterVariableId() const;

 private:
  void fillOverrideMask(uint8_t overriddenBoneId);

  void linearBlendPoses(const SkeletalAnimationStatesMachineVariables& variablesSet);
  void overriddenBlendPoses(const SkeletalAnimationStatesMachineVariables& variablesSet);

 private:
  SkeletalAnimationClipInstance m_firstClip;
  SkeletalAnimationClipInstance m_secondClip;

  SkeletalAnimationBlendPoseType m_blendType;
  SkeletalAnimationVariableId m_blendParameterVariableId;

  std::vector<uint8_t> m_overrideMask;

  SkeletalAnimationPose m_blendedPose;
};

class SkeletalAnimationState {
 public:
  SkeletalAnimationState(const std::string& name, std::unique_ptr<SkeletalAnimationPoseNode> initialPoseNode);

  [[nodiscard]] const std::string& getName() const;

  void setFinalAction(SkeletalAnimationFinalAction action);
  [[nodiscard]] SkeletalAnimationFinalAction getFinalAction() const;

  void setFinalTransitionStateId(int16_t stateId);
  [[nodiscard]] int16_t getFinalTransitionStateId() const;

  [[nodiscard]] bool hasTransition(int16_t nextStateId) const;

  void increaseCurrentTime(float delta, const SkeletalAnimationStatesMachineVariables& variablesSet);
  void resetCurrentTime();

  [[nodiscard]] const SkeletalAnimationPose& getCurrentPose() const;

 private:
  int16_t m_id;
  std::string m_name;

  std::unordered_set<int16_t> m_nextTransitions;

  SkeletalAnimationFinalAction m_finalAction = SkeletalAnimationFinalAction::Stop;
  int16_t m_finalTransitionStateId;

  std::unique_ptr<SkeletalAnimationPoseNode> m_initialPoseNode;

 private:
  friend class SkeletalAnimationStatesMachine;
};
