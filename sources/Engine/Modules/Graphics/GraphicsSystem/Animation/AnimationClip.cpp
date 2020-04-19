#include "precompiled.h"

#pragma hdrstop

#include "AnimationClip.h"
#include "Exceptions/exceptions.h"
#include "Bone.h"
#include "AnimationClipInstance.h"

AnimationClip::AnimationClip(const std::string& name,
        float duration,
        float rate,
        const std::vector<BoneAnimationChannel>& bonesAnimationChannels)
        : m_name(name),
          m_bonesAnimationChannels(bonesAnimationChannels),
          m_duration(duration),
          m_rate(rate)
{

}

const std::string& AnimationClip::getName() const
{
    return m_name;
}

void AnimationClip::setDuration(float duration)
{
    m_duration = duration;
}

float AnimationClip::getDuration() const
{
    return m_duration;
}

void AnimationClip::setRate(float rate)
{
    m_rate = rate;
}

float AnimationClip::getRate() const
{
    return m_rate;
}

BonePose AnimationClip::getBoneRelativePose(uint8_t boneIndex, float time) const
{
    const std::vector<BoneAnimationPositionFrame>& positionFrames =
            m_bonesAnimationChannels[boneIndex].getPositionFrames();

    glm::vec3 position = getMixedAdjacentFrames<glm::vec3, BoneAnimationPositionFrame>(positionFrames, time);

    const std::vector<BoneAnimationOrientationFrame>& orientationFrames =
            m_bonesAnimationChannels[boneIndex].getOrientationFrames();

    glm::quat orientation = getMixedAdjacentFrames<glm::quat, BoneAnimationOrientationFrame>(orientationFrames, time);

    return BonePose(position, orientation);
}

AnimationMatrixPalette::AnimationMatrixPalette(const std::vector<glm::mat4>& bonesTransforms)
        : bonesTransforms(bonesTransforms)
{

}

template<class T, class S>
T AnimationClip::getMixedAdjacentFrames(const std::vector<S>& frames, float time) const
{
    S tempFrame;
    tempFrame.time = time;

    auto frameIt = std::upper_bound(frames.begin(), frames.end(),
            tempFrame, [](const S& a, const S& b) {
              return a.time < b.time;
            });

    if (frameIt == frames.end()) {
        return (frames.size() > 0) ? getFrameValue<T, S>(*frames.rbegin()) : getIdentity<T>();
    }
    else {
        T next = getFrameValue<T, S>(*frameIt);
        T prev = (frameIt == frames.begin()) ? getIdentity<T>() : getFrameValue<T, S>(*std::prev(frameIt));

        float currentFrameTime = frameIt->time;
        float prevFrameTime = (frameIt == frames.begin()) ? 0 : std::prev(frameIt)->time;

        float framesTimeDelta = currentFrameTime - prevFrameTime;

        return getMixedValue<T>(prev, next, (time - prevFrameTime) / framesTimeDelta);
    }
}

template<>
glm::vec3 AnimationClip::getIdentity() const
{
    return glm::vec3(0.0f);
}

template<>
glm::quat AnimationClip::getIdentity() const
{
    return glm::vec3(0.0f);
}

template<>
glm::vec3 AnimationClip::getFrameValue(const BoneAnimationPositionFrame& frame) const
{
    return frame.position;
}

template<>
glm::quat AnimationClip::getFrameValue(const BoneAnimationOrientationFrame& frame) const
{
    return frame.orientation;
}

template<>
glm::vec3 AnimationClip::getMixedValue(const glm::vec3& first, const glm::vec3& second, float delta) const
{
    return glm::mix(first, second, delta);
}

template<>
glm::quat AnimationClip::getMixedValue(const glm::quat& first, const glm::quat& second, float delta) const
{
    return glm::slerp(first, second, delta);
}
