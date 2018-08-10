#pragma once

#include "Bone.h"

class Skeleton {
public:
	Skeleton(const std::vector<Bone>& bones, const matrix4& globalInverseTransform);
	~Skeleton();

	Bone* getBone(size_t id) const;

	size_t getBonesCount() const;
	const std::vector<Bone>& getBones() const;

	Bone* getRootBone() const;
	std::vector<Bone*> getChildBones(size_t boneId) const;

	void resetPose();

	const matrix4& getGlobalInverseTransform() const;
private:
	std::vector<Bone> m_bones;

	Bone* m_rootBone;

	matrix4 m_globalInverseTransform;
};