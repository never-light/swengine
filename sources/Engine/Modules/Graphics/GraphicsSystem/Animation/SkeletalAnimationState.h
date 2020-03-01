#pragma once

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

    virtual const SkeletalAnimationPose& getCurrentPose() const = 0;
};

class SkeletalAnimationClipPoseNode : public SkeletalAnimationPoseNode {
public:
    SkeletalAnimationClipPoseNode(const SkeletalAnimationClipInstance& clip);
    ~SkeletalAnimationClipPoseNode() override = default;

    const SkeletalAnimationPose& getCurrentPose() const override;

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
                                   size_t blendParameterVariableId,
                                   SkeletalAnimationBlendPoseType blendType = SkeletalAnimationBlendPoseType::Linear,
                                   uint8_t overriddenBone = 0);

    ~SkeletalAnimationBlendPoseNode() override;

    const SkeletalAnimationPose& getCurrentPose() const override;

    void increaseCurrentTime(float delta,
                             const SkeletalAnimationStatesMachineVariables& variablesSet) override;

    void setBlendParameterVariableId(size_t variableId);
    size_t getBlendParameterVariableId() const;

private:
    void fillOverrideMask(uint8_t overriddenBoneId);

private:
    SkeletalAnimationClipInstance m_firstClip;
    SkeletalAnimationClipInstance m_secondClip;

    SkeletalAnimationBlendPoseType m_blendType;
    size_t m_blendParameterVariableId;

    std::vector<uint8_t> m_overrideMask;

    SkeletalAnimationPose m_blendedPose;
};

class SkeletalAnimationState {
public:
    SkeletalAnimationState(const std::string& name, std::unique_ptr<SkeletalAnimationPoseNode> initialPoseNode);

    const std::string& getName() const;

    void setFinalAction(SkeletalAnimationFinalAction action);
    SkeletalAnimationFinalAction getFinalAction() const;

    void setFinalTransitionStateId(int16_t stateId);
    int16_t getFinalTransitionStateId() const;

    bool hasTransition(int16_t nextStateId) const;

    void increaseCurrentTime(float delta, const SkeletalAnimationStatesMachineVariables& variablesSet);
    void resetCurrentTime();

    const SkeletalAnimationPose& getCurrentPose() const;

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
