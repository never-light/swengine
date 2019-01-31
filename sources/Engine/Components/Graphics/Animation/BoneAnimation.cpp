#include "BoneAnimation.h"

BoneAnimation::BoneAnimation(size_t boneIndex,
	const std::vector<BonePositionKeyFrame>& positionKeyFrames,
	const std::vector<BoneOrientationKeyFrame>& orientationKeyFrames)
	: m_boneIndex(boneIndex), 
	m_positionKeyFrames(positionKeyFrames),
	m_orientationKeyFrames(orientationKeyFrames)
{
}

BoneAnimation::~BoneAnimation()
{
}

size_t BoneAnimation::getBoneIndex() const
{
	return m_boneIndex;
}

const std::vector<BonePositionKeyFrame>& BoneAnimation::getPositionKeyFrames() const
{
	return m_positionKeyFrames;
}

const std::vector<BoneOrientationKeyFrame>& BoneAnimation::getOrientationKeyFrames() const
{
	return m_orientationKeyFrames;
}
