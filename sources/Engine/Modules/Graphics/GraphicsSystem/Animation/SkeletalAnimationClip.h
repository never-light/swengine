#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Skeleton.h"

struct BoneAnimationPositionFrame {
    float time;
    glm::vec3 position;
};

struct BoneAnimationOrientationFrame {
    float time;
    glm::quat orientation;
};

struct SkeletalAnimationMatrixPalette {
    SkeletalAnimationMatrixPalette(const std::vector<glm::mat4>& bonesTransforms);

    std::vector<glm::mat4> bonesTransforms;
};

class BoneAnimationChannel {
public:
    BoneAnimationChannel() = default;
    BoneAnimationChannel(const std::vector<BoneAnimationPositionFrame>& positionFrames,
                         const std::vector<BoneAnimationOrientationFrame>& orientationFrames);

    ~BoneAnimationChannel() = default;

    std::vector<BoneAnimationPositionFrame>& getPositionFrames();
    const std::vector<BoneAnimationPositionFrame>& getPositionFrames() const;

    const std::vector<BoneAnimationOrientationFrame>& getOrientationFrames() const;
    std::vector<BoneAnimationOrientationFrame>& getOrientationFrames();

private:
    std::vector<BoneAnimationPositionFrame> m_positionFrames;
    std::vector<BoneAnimationOrientationFrame> m_orientationFrames;
};

class SkeletalAnimationClip
{
public:
    SkeletalAnimationClip(const std::string& name,
                          float duration,
                          float rate,
                          const std::vector<BoneAnimationChannel>& bonesAnimationChannels);

    SkeletalAnimationClip(const SkeletalAnimationClip& clip);

    ~SkeletalAnimationClip() = default;

    const std::string& getName() const;

    void setDuration(float duration);
    float getDuration() const;

    void setRate(float rate);
    float getRate() const;

    BonePose getBoneRelativePose(uint8_t boneIndex, float time) const;

private:
    template<class T, class S>
    T getMixedAdjacentFrames(const std::vector<S>& frames, float time) const;

    template<class T>
    T getIdentity() const;

    template<class T, class S>
    T getFrameValue(const S& frame) const;

    template<class T>
    T getMixedValue(const T& first, const T& second, float delta) const;

private:
    std::string m_name;
    std::vector<BoneAnimationChannel> m_bonesAnimationChannels;

    float m_duration = 0.0f;
    float m_rate = 0.0f;
};

class SkeletalAnimationClipInstance;

class SkeletalAnimationPose {
public:
    SkeletalAnimationPose(std::shared_ptr<Skeleton> skeleton);

    SkeletalAnimationPose(std::shared_ptr<Skeleton> skeleton,
            const std::vector<BonePose>& bonesPoses);

    void setBoneLocalPose(uint8_t boneIndex, const BonePose& pose);
    const BonePose& getBoneLocalPose(uint8_t boneIndex) const;

    const SkeletalAnimationMatrixPalette& getMatrixPalette() const;

private:
    std::shared_ptr<Skeleton> m_skeleton;

    std::vector<BonePose> m_bonesLocalPoses;

    mutable SkeletalAnimationMatrixPalette m_matrixPalette;
    mutable bool m_isMatrixPaletteOutdated = true;

private:
    friend class SkeletalAnimationClipInstance;
};

enum class SkeletalAnimationClipEndBehaviour {
    Repeat, Stop, StopAndReset
};

enum class SkeletalAnimationClipState {
    Active, Paused
};

class SkeletalAnimationClipInstance {
public:
    SkeletalAnimationClipInstance(std::shared_ptr<Skeleton> skeleton,
                                  std::shared_ptr<SkeletalAnimationClip> animationClip);

    const SkeletalAnimationClip& getAnimationClip() const;
    SkeletalAnimationClip& getAnimationClip();

    const SkeletalAnimationPose& getAnimationPose() const;
    const Skeleton& getSkeleton() const;

    std::shared_ptr<Skeleton> getSkeletonPtr() const;

    void resetAnimationPose();

    void increaseCurrentTime(float delta);
    void resetCurrentTime();

    float getCurrentTime() const;

    void setScale(float scale);
    float getScale() const;

    void start();
    void pause();

private:
    std::shared_ptr<Skeleton> m_skeleton;

    std::shared_ptr<SkeletalAnimationClip> m_animationClip;
    mutable SkeletalAnimationPose m_animationPose;
    mutable bool m_isAnimationPoseOutdated = true;

    float m_scale = 1.0f;
    float m_currentTime = 0.0f;

    SkeletalAnimationClipEndBehaviour m_endBehaviour = SkeletalAnimationClipEndBehaviour::Stop;
    SkeletalAnimationClipState m_clipState = SkeletalAnimationClipState::Active;
};
