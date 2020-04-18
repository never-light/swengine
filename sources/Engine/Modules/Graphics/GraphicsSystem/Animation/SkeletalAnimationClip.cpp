#include "precompiled.h"

#pragma hdrstop

#include "SkeletalAnimationClip.h"
#include "Exceptions/exceptions.h"

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

std::shared_ptr<Skeleton> SkeletalAnimationClipInstance::getSkeletonPtr() const
{
    return m_skeleton;
}

void SkeletalAnimationClipInstance::resetAnimationPoseCache()
{
    m_isAnimationPoseOutdated = true;
}

void SkeletalAnimationClipInstance::increaseCurrentTime(float delta)
{
    if (m_clipState == SkeletalAnimationClipState::Paused) {
        return;
    }

    m_currentTime += delta * m_animationClip->getRate() * m_scale;

    if (m_currentTime > m_animationClip->getDuration()) {
        if (m_endBehaviour == SkeletalAnimationClipEndBehaviour::Stop) {
            m_currentTime = m_animationClip->getDuration();
            m_clipState = SkeletalAnimationClipState::Paused;
        }
        else if (m_endBehaviour == SkeletalAnimationClipEndBehaviour::StopAndReset) {
            m_currentTime = 0.0f;
            m_clipState = SkeletalAnimationClipState::Paused;
        }
        else if (m_endBehaviour == SkeletalAnimationClipEndBehaviour::Repeat) {
            int overflowParts = static_cast<int>(m_currentTime / m_animationClip->getDuration());
            m_currentTime -= m_animationClip->getDuration() * static_cast<float>(overflowParts);
        }
        else {
            THROW_EXCEPTION(NotImplementedException, "Clip behaviour is not impelented");
        }
    }

    resetAnimationPoseCache();
}

void SkeletalAnimationClipInstance::resetCurrentTime()
{
    m_currentTime = 0.0f;
}

float SkeletalAnimationClipInstance::getCurrentTime() const
{
    return m_currentTime;
}

void SkeletalAnimationClipInstance::setScale(float scale)
{
    m_scale = scale;
}

float SkeletalAnimationClipInstance::getScale() const
{
    return m_scale;
}

void SkeletalAnimationClipInstance::start()
{
    m_clipState = SkeletalAnimationClipState::Active;
}

void SkeletalAnimationClipInstance::pause()
{
    m_clipState = SkeletalAnimationClipState::Paused;
}

void SkeletalAnimationClipInstance::setEndBehaviour(SkeletalAnimationClipEndBehaviour behaviour)
{
    m_endBehaviour = behaviour;
}

const SkeletalAnimationClip& SkeletalAnimationClipInstance::getAnimationClip() const
{
    return *m_animationClip.get();
}

SkeletalAnimationClip& SkeletalAnimationClipInstance::getAnimationClip()
{
    return *m_animationClip.get();
}

const SkeletalAnimationPose& SkeletalAnimationClipInstance::getAnimationPose() const
{
    if (!m_isAnimationPoseOutdated) {
        return m_animationPose;
    }

    // Update animation pose
    m_animationPose.setBoneLocalPose(0, m_animationClip->getBoneRelativePose(0, m_currentTime));

    for (uint8_t boneIndex = 1; boneIndex < m_skeleton->getBonesCount(); boneIndex++) {
        BonePose currentLocalPose = m_animationClip->getBoneRelativePose(boneIndex, m_currentTime);
        m_animationPose.setBoneLocalPose(boneIndex, currentLocalPose);
    }

    m_isAnimationPoseOutdated = false;
    return m_animationPose;
}

SkeletalAnimationPose::SkeletalAnimationPose(std::shared_ptr<Skeleton> skeleton)
        : m_skeleton(skeleton),
          m_bonesLocalPoses(std::vector<BonePose>(skeleton->getBonesCount())),
          m_matrixPalette(std::vector<glm::mat4>(skeleton->getBonesCount(), glm::identity<glm::mat4>()))
{

}

SkeletalAnimationPose::SkeletalAnimationPose(std::shared_ptr<Skeleton> skeleton,
        const std::vector<BonePose>& bonesPoses)
        : m_skeleton(skeleton),
          m_bonesLocalPoses(bonesPoses),
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

const SkeletalAnimationMatrixPalette& SkeletalAnimationPose::getMatrixPalette() const
{
    if (m_isMatrixPaletteOutdated) {
        // Update matrix palette hierarchy
        m_matrixPalette.bonesTransforms[0] = m_bonesLocalPoses[0].getBoneMatrix();

        for (uint8_t boneIndex = 1; boneIndex < m_matrixPalette.bonesTransforms.size(); boneIndex++) {
            m_matrixPalette.bonesTransforms[boneIndex] =
                    m_matrixPalette.bonesTransforms[m_skeleton->getBoneParentId(boneIndex)] *
                            m_bonesLocalPoses[boneIndex].getBoneMatrix();
        }

        // Multiply matrix palette by inverse bind pose matrices
        for (uint8_t boneIndex = 0; boneIndex < m_matrixPalette.bonesTransforms.size(); boneIndex++) {
            m_matrixPalette.bonesTransforms[boneIndex] *= m_skeleton->getBone(boneIndex).getInverseBindPoseMatrix();
        }

        m_isMatrixPaletteOutdated = false;
    }

    return m_matrixPalette;
}

SkeletalAnimationMatrixPalette::SkeletalAnimationMatrixPalette(const std::vector<glm::mat4>& bonesTransforms)
        : bonesTransforms(bonesTransforms)
{

}

template<class T, class S>
T SkeletalAnimationClip::getMixedAdjacentFrames(const std::vector<S>& frames, float time) const
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
