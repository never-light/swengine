#include "precompiled.h"
#pragma hdrstop

#include "SkeletalAnimationClip.h"

BoneAnimationChannel::BoneAnimationChannel(const std::vector<BoneAnimationPositionFrame>& positionFrames,
                                           const std::vector<BoneAnimationOrientationFrame>& orientationFrames)
    : m_positionFrames(positionFrames),
      m_orientationFrames(orientationFrames)
{

}

std::vector<BoneAnimationPositionFrame>& BoneAnimationChannel::getPositionFrames()
{
    return m_positionFrames;
}

const std::vector<BoneAnimationPositionFrame>& BoneAnimationChannel::getPositionFrames() const
{
    return m_positionFrames;
}

const std::vector<BoneAnimationOrientationFrame>& BoneAnimationChannel::getOrientationFrames() const
{
    return m_orientationFrames;
}

std::vector<BoneAnimationOrientationFrame>& BoneAnimationChannel::getOrientationFrames()
{
    return m_orientationFrames;
}

SkeletalAnimationClip::SkeletalAnimationClip(const std::string& name,
                                             float duration,
                                             float rate,
                                             const std::vector<BoneAnimationChannel>& bonesAnimationChannels)
    : m_name(name),
      m_bonesAnimationChannels(bonesAnimationChannels),
      m_duration(duration),
      m_rate(rate)
{

}

const std::string& SkeletalAnimationClip::getName() const
{
    return m_name;
}

void SkeletalAnimationClip::setDuration(float duration)
{
    m_duration = duration;
}

float SkeletalAnimationClip::getDuration() const
{
    return m_duration;
}

void SkeletalAnimationClip::setRate(float rate)
{
    m_rate = rate;
}

float SkeletalAnimationClip::getRate() const
{
    return m_rate;
}

BonePose SkeletalAnimationClip::getBoneRelativePose(uint8_t boneIndex, float time) const
{
    const std::vector<BoneAnimationPositionFrame>& positionFrames =
            m_bonesAnimationChannels[boneIndex].getPositionFrames();

    glm::vec3 position = getMixedAdjacentFrames<glm::vec3, BoneAnimationPositionFrame>(positionFrames, time);

    const std::vector<BoneAnimationOrientationFrame>& orientationFrames =
            m_bonesAnimationChannels[boneIndex].getOrientationFrames();

    glm::quat orientation = getMixedAdjacentFrames<glm::quat, BoneAnimationOrientationFrame>(orientationFrames, time);

    return BonePose(position, orientation);
}

SkeletalAnimationClipInstance::SkeletalAnimationClipInstance(std::shared_ptr<Skeleton> skeleton,
                                                             std::shared_ptr<SkeletalAnimationClip> animationClip)
    : m_skeleton(skeleton),
      m_animationClip(animationClip),
      m_animationPose(skeleton, std::vector<BonePose>(skeleton->getBonesCount()))
{

}

const Skeleton& SkeletalAnimationClipInstance::getSkeleton() const
{
    return *m_skeleton.get();
}

void SkeletalAnimationClipInstance::increaseCurrentTime(float delta)
{
    m_currentTime += delta * m_animationClip->getRate() * m_scale;
    m_isAnimationPoseOutdated = true;

    if (m_currentTime > m_animationClip->getDuration()) {
        int overflowParts = static_cast<int>(m_currentTime / m_animationClip->getDuration());
        m_currentTime -= m_animationClip->getDuration() * overflowParts;
    }
}

float SkeletalAnimationClipInstance::getCurrentTime() const
{
    return m_currentTime;
}

const SkeletalAnimationClip& SkeletalAnimationClipInstance::getAnimationClip() const
{
    return *m_animationClip.get();
}

SkeletalAnimationClip& SkeletalAnimationClipInstance::getAnimationClip()
{
    return *m_animationClip.get();
}

void SkeletalAnimationClipInstance::setScale(float scale)
{
    m_scale = scale;
}

float SkeletalAnimationClipInstance::getScale() const
{
    return m_scale;
}

void SkeletalAnimationClipInstance::setLooped(bool looped)
{
    m_isLooped = looped;
}

bool SkeletalAnimationClipInstance::isLooped() const
{
    return m_isLooped;
}

const SkeletalAnimationPose& SkeletalAnimationClipInstance::getAnimationPose() const
{
    if (!m_isAnimationPoseOutdated) {
        return m_animationPose;
    }

    // Update animation pose
    m_animationPose.setBoneLocalPose(0, m_animationClip->getBoneRelativePose(0, m_currentTime));
    m_animationPose.updateBoneGlobalPose(0, m_animationClip->getBoneRelativePose(0, m_currentTime));

    for (uint8_t boneIndex = 1; boneIndex < m_skeleton->getBonesCount(); boneIndex++) {
        BonePose currentLocalPose = m_animationClip->getBoneRelativePose(boneIndex, m_currentTime);
        m_animationPose.setBoneLocalPose(boneIndex, currentLocalPose);

        const BonePose& parentGlobalPose = m_animationPose.getBoneGlobalPose(
                    m_skeleton->getBoneParentId(boneIndex));

        BonePose currentGlobalPose;
        currentGlobalPose.position = parentGlobalPose.position + currentLocalPose.position;
        currentGlobalPose.orientation = parentGlobalPose.orientation * currentLocalPose.orientation;

        m_animationPose.updateBoneGlobalPose(boneIndex, currentGlobalPose);
    }

    m_isAnimationPoseOutdated = false;
    return m_animationPose;
}

SkeletalAnimationPose::SkeletalAnimationPose(std::shared_ptr<Skeleton> skeleton,
                                             const std::vector<BonePose>& bonesPoses)
    : m_skeleton(skeleton),
      m_bonesLocalPoses(bonesPoses),
      m_bonesGlobalPoses(bonesPoses.size()),
      m_matrixPalette(std::vector<glm::mat4>(bonesPoses.size(), glm::identity<glm::mat4>()))
{

}

void SkeletalAnimationPose::setBoneLocalPose(uint8_t boneIndex, const BonePose& pose)
{
    m_bonesLocalPoses[boneIndex] = pose;
    m_isMatrixPaletteOutdated = true;
}

const BonePose& SkeletalAnimationPose::getBoneLocalPose(uint8_t boneIndex) const
{
    return m_bonesLocalPoses[boneIndex];
}

#include <spdlog/spdlog.h>
#include <glm/gtx/string_cast.hpp>

const SkeletalAnimationMatrixPalette& SkeletalAnimationPose::getMatrixPalette() const
{
    if (m_isMatrixPaletteOutdated) {        
        for (size_t boneIndex = 0; boneIndex < m_matrixPalette.bonesTransforms.size(); boneIndex++) {
            m_matrixPalette.bonesTransforms[boneIndex] = glm::mat4_cast(m_bonesGlobalPoses[boneIndex].orientation);
            m_matrixPalette.bonesTransforms[boneIndex] = glm::translate(m_matrixPalette.bonesTransforms[boneIndex],
                                                                        m_bonesGlobalPoses[boneIndex].position);

           m_matrixPalette.bonesTransforms[boneIndex] =
                     m_matrixPalette.bonesTransforms[boneIndex] * m_skeleton->getBone(static_cast<uint8_t>(boneIndex)).getInverseBindPoseMatrix();
        }

        m_isMatrixPaletteOutdated = false;
    }

    return m_matrixPalette;
}

const BonePose& SkeletalAnimationPose::getBoneGlobalPose(uint8_t boneIndex) const
{
    return m_bonesGlobalPoses[boneIndex];
}

void SkeletalAnimationPose::updateBoneGlobalPose(uint8_t boneIndex, const BonePose& pose)
{
    m_bonesGlobalPoses[boneIndex] = pose;
}

SkeletalAnimationMatrixPalette::SkeletalAnimationMatrixPalette(const std::vector<glm::mat4>& bonesTransforms)
    : bonesTransforms(bonesTransforms)
{

}

BonePose::BonePose(const glm::vec3& position, const glm::quat& orientation)
    : position(position),
      orientation(orientation)
{

}

template<class T, class S>
T SkeletalAnimationClip::getMixedAdjacentFrames(const std::vector<S>& frames, float time) const
{
    S tempFrame;
    tempFrame.time = time;

    auto frameIt = std::upper_bound(frames.begin(), frames.end(),
        tempFrame, [](const S& a, const S& b)
    {
        return a.time < b.time;
    });

    if (frameIt == frames.end()) {
        return (frames.size() > 0) ? getFrameValue<T, S>(*frames.rbegin()) : getIdentity<T>();
    }
    else {
        T next = getFrameValue<T, S>(*frameIt);
        T prev = (frameIt == frames.begin()) ? getIdentity<T>() : getFrameValue<T, S>(*std::prev(frameIt));

        return getMixedValue<T>(prev, next, time / frameIt->time);
    }
}

template<>
glm::vec3 SkeletalAnimationClip::getIdentity() const
{
    return glm::vec3(0.0f);
}

template<>
glm::quat SkeletalAnimationClip::getIdentity() const
{
    return glm::vec3(0.0f);
}

template<>
glm::vec3 SkeletalAnimationClip::getFrameValue(const BoneAnimationPositionFrame& frame) const
{
    return frame.position;
}

template<>
glm::quat SkeletalAnimationClip::getFrameValue(const BoneAnimationOrientationFrame& frame) const
{
    return frame.orientation;
}

template<>
glm::vec3 SkeletalAnimationClip::getMixedValue(const glm::vec3& first, const glm::vec3& second, float delta) const
{
    return glm::mix(first, second, delta);
}

template<>
glm::quat SkeletalAnimationClip::getMixedValue(const glm::quat& first, const glm::quat& second, float delta) const
{
    return glm::slerp(first, second, delta);
}
