#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Skeleton.h"

struct BoneAnimationPositionFrame {
    BoneAnimationPositionFrame() = default;
    BoneAnimationPositionFrame(float time, const glm::vec3& position);

    float time = 0.0f;
    glm::vec3 position = glm::vec3(0.0f);
};

struct BoneAnimationOrientationFrame {
    BoneAnimationOrientationFrame() = default;
    BoneAnimationOrientationFrame(float time, const glm::quat& orientation);

    float time = 0.0f;
    glm::quat orientation = glm::identity<glm::quat>();
};

struct BonePose {
    BonePose() = default;
    BonePose(const glm::vec3& position, const glm::quat& orientation);

    glm::vec3 position = glm::vec3(0.0f);
    glm::quat orientation = glm::identity<glm::quat>();
};

struct SkeletalAnimationMatrixPalette {
    SkeletalAnimationMatrixPalette(const std::vector<glm::mat4>& bonesTransforms);

    std::vector<glm::mat4> bonesTransforms;
};

class BoneAnimationChannel {
public:
    BoneAnimationChannel(const std::vector<BoneAnimationPositionFrame>& positionFrames,
                         const std::vector<BoneAnimationOrientationFrame>& orientationFrames);

    ~BoneAnimationChannel() = default;

    const std::vector<BoneAnimationPositionFrame>& getPositionFrames() const;
    const std::vector<BoneAnimationOrientationFrame> getOrientationFrames() const;

private:
    std::vector<BoneAnimationPositionFrame> m_positionFrames;
    std::vector<BoneAnimationOrientationFrame> m_orientationFrames;
};

class SkeletalAnimationClip
{
public:
    SkeletalAnimationClip(std::shared_ptr<Skeleton> skeleton,
                          const std::string& name,
                          float duration,
                          float rate,
                          const std::vector<BoneAnimationChannel>& bonesAnimationChannels);

    ~SkeletalAnimationClip() = default;

    const std::string& getName() const;

    void setDuration(float duration);
    float getDuration() const;

    void setRate(float rate);
    float getRate() const;

    const Skeleton& getSkeleton() const;

private:
    std::shared_ptr<Skeleton> m_skeleton;

    std::string m_name;
    std::vector<BoneAnimationChannel> m_bonesAnimationChannels;

    float m_duration = 0.0f;
    float m_rate = 0.0f;
};

class SkeletalAnimationPose {
public:
    SkeletalAnimationPose(const std::vector<BonePose>& bonesPoses);

    void setBonePose(uint8_t boneIndex, const BonePose& pose);

    const BonePose& getBonePose(uint8_t boneIndex) const;

    const SkeletalAnimationMatrixPalette& getMatrixPalette() const;

private:
    std::vector<BonePose> m_bonesPoses;
    mutable SkeletalAnimationMatrixPalette m_matrixPalette;
    mutable bool m_isMatrixPaletteOutdated = true;
};

class SkeletalAnimationClipInstance {
public:
    SkeletalAnimationClipInstance(std::shared_ptr<SkeletalAnimationClip> animationClip);

    const SkeletalAnimationClip& getAnimationClip() const;
    SkeletalAnimationClip& getAnimationClip();

    void setScale(float scale);
    float getScale() const;

    void setLooped(bool looped);
    bool isLooped() const;

    const SkeletalAnimationPose& getAnimationPose() const;
    SkeletalAnimationPose& getAnimationPose();

private:
    std::shared_ptr<SkeletalAnimationClip> m_animationClip;
    SkeletalAnimationPose m_animationPose;

    float m_scale = 1.0f;
    bool m_isLooped = false;
};
