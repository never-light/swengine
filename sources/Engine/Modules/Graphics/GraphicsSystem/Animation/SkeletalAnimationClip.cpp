#include "precompiled.h"
#pragma hdrstop

#include "SkeletalAnimationClip.h"

BoneAnimationChannel::BoneAnimationChannel(const std::vector<BoneAnimationPositionFrame>& positionFrames,
                                           const std::vector<BoneAnimationOrientationFrame>& orientationFrames)
    : m_positionFrames(positionFrames),
      m_orientationFrames(orientationFrames)
{

}

const std::vector<BoneAnimationPositionFrame>& BoneAnimationChannel::getPositionFrames() const
{
    return m_positionFrames;
}

const std::vector<BoneAnimationOrientationFrame> BoneAnimationChannel::getOrientationFrames() const
{
    return m_orientationFrames;
}

SkeletalAnimationClip::SkeletalAnimationClip(std::shared_ptr<Skeleton> skeleton,
                                             const std::string& name,
                                             float duration,
                                             float rate,
                                             const std::vector<BoneAnimationChannel>& bonesAnimationChannels)
    : m_skeleton(skeleton),
      m_name(name),
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

const Skeleton& SkeletalAnimationClip::getSkeleton() const
{
    return *m_skeleton.get();
}

SkeletalAnimationClipInstance::SkeletalAnimationClipInstance(std::shared_ptr<SkeletalAnimationClip> animationClip)
    : m_animationClip(animationClip),
      m_animationPose(std::vector<BonePose>(animationClip->getSkeleton().getBonesCount()))
{

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
    return m_animationPose;
}

SkeletalAnimationPose& SkeletalAnimationClipInstance::getAnimationPose()
{
    return m_animationPose;
}

SkeletalAnimationPose::SkeletalAnimationPose(const std::vector<BonePose>& bonesPoses)
    : m_bonesPoses(bonesPoses),
      m_matrixPalette(std::vector<glm::mat4>(bonesPoses.size(), glm::identity<glm::mat4>()))
{

}

void SkeletalAnimationPose::setBonePose(uint8_t boneIndex, const BonePose& pose)
{
    m_bonesPoses[boneIndex] = pose;
    m_isMatrixPaletteOutdated = true;
}

const BonePose& SkeletalAnimationPose::getBonePose(uint8_t boneIndex) const
{
    return m_bonesPoses[boneIndex];
}

const SkeletalAnimationMatrixPalette& SkeletalAnimationPose::getMatrixPalette() const
{
    if (m_isMatrixPaletteOutdated) {
        for (size_t boneIndex = 0; boneIndex < m_matrixPalette.bonesTransforms.size(); boneIndex++) {
            m_matrixPalette.bonesTransforms[boneIndex] = glm::mat4_cast(m_bonesPoses[boneIndex].orientation);
            m_matrixPalette.bonesTransforms[boneIndex] = glm::translate(m_matrixPalette.bonesTransforms[boneIndex], m_bonesPoses[boneIndex].position);
        }

        m_isMatrixPaletteOutdated = false;
    }

    return m_matrixPalette;
}


BoneAnimationPositionFrame::BoneAnimationPositionFrame(float time, const glm::vec3& position)
    : time(time), position(position)
{

}

BoneAnimationOrientationFrame::BoneAnimationOrientationFrame(float time, const glm::quat& orientation)
    : time(time), orientation(orientation)
{

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
