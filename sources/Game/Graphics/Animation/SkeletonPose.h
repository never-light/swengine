#pragma once

#include <vector>
#include <Engine\Components\Math\types.h>

class SkeletonPose {
public:
	SkeletonPose(size_t bonesCount);
	~SkeletonPose();

	void reset();

	void setBoneTransform(size_t boneId, const matrix4& transform);

	bool isBoneAffected(size_t boneId) const;
	const matrix4& getBoneTransform(size_t boneId) const;
private:
	std::vector<matrix4> m_currentPoseTransforms;
	std::vector<bool> m_currentPoseTransformsMarks;
};