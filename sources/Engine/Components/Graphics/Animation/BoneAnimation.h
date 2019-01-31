#pragma once

#include <vector>
#include <Engine/Components/Math/types.h>

struct BonePositionKeyFrame {
	float timeStamp;
	vector3 position;
};

struct BoneOrientationKeyFrame {
	float timeStamp;
	quaternion orientation;
};

class BoneAnimation {
public:
	BoneAnimation(size_t boneIndex, 
		const std::vector<BonePositionKeyFrame>& positionKeyFrames,
		const std::vector<BoneOrientationKeyFrame>& orientationKeyFrames);
	~BoneAnimation();

	size_t getBoneIndex() const;

	const std::vector<BonePositionKeyFrame>& getPositionKeyFrames() const;
	const std::vector<BoneOrientationKeyFrame>& getOrientationKeyFrames() const;

private:
	size_t m_boneIndex;

	std::vector<BonePositionKeyFrame> m_positionKeyFrames;
	std::vector<BoneOrientationKeyFrame> m_orientationKeyFrames;
};