#include "SkeletonPose.h"

SkeletonPose::SkeletonPose(size_t bonesCount)
	: m_currentPoseTransforms(bonesCount), m_currentPoseTransformsMarks(bonesCount, false)
{

}

SkeletonPose::~SkeletonPose()
{
}

void SkeletonPose::reset()
{
	std::fill(m_currentPoseTransformsMarks.begin(), m_currentPoseTransformsMarks.end(), false);
}

void SkeletonPose::setBoneTransform(size_t boneId, const matrix4 & transform)
{
	m_currentPoseTransforms[boneId] = transform;
	m_currentPoseTransformsMarks[boneId] = true;
}

bool SkeletonPose::isBoneAffected(size_t boneId) const
{
	return m_currentPoseTransformsMarks[boneId];
}

const matrix4 & SkeletonPose::getBoneTransform(size_t boneId) const
{
	return m_currentPoseTransforms[boneId];
}
