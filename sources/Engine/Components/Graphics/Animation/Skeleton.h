#pragma once

#include "Bone.h"
#include "SkeletonPose.h"

class Skeleton {
public:
	Skeleton(const std::vector<Bone>& bones, const matrix4& globalInverseTransform);
	~Skeleton();

	Bone* getBone(size_t id) const;
	Bone* getBone(const std::string& name) const;

	size_t getBonesCount() const;
	const std::vector<Bone>& getBones() const;

	Bone* getRootBone() const;
	std::vector<Bone*> getChildBones(size_t boneId) const;

	void resetPose();

	const matrix4& getGlobalInverseTransform() const;

	void applyPose(const SkeletonPose& pose);

	const matrix4& getLocalBoneTransform(size_t boneId) const;
private:
	void applyTransformsToBonesHierarchy(const SkeletonPose& pose, Bone* bone, const matrix4& parentTransform);
	
private:
	std::vector<Bone> m_bones;

	Bone* m_rootBone;
	matrix4 m_globalInverseTransform;
}; 