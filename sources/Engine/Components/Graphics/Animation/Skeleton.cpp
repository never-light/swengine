#include "Skeleton.h"

#include <Engine/Exceptions/EngineException.h>

Skeleton::Skeleton(const std::vector<Bone>& bones, const matrix4& globalInverseTransform)
	: m_bones(bones), m_globalInverseTransform(globalInverseTransform)
{
	bool rootBoneFound = false;

	for (Bone& bone : m_bones) {
		if (bone.isRoot()) {
			m_rootBone = &bone;
			rootBoneFound = true;
			break;
		}
	}

	if (!rootBoneFound)
		throw EngineException("Skeleton must contains root bone", __FILE__, __LINE__, __FUNCTION__);
}

Skeleton::~Skeleton()
{
}

Bone* Skeleton::getBone(size_t id) const
{
	return const_cast<Bone*>(&m_bones[id]);
}

Bone * Skeleton::getBone(const std::string & name) const
{
	for (const Bone& bone : m_bones)
		if (bone.getName() == name)
			return const_cast<Bone*>(&bone);

	return nullptr;
}

size_t Skeleton::getBonesCount() const
{
	return m_bones.size();
}

const std::vector<Bone>& Skeleton::getBones() const
{
	return m_bones;
}

Bone* Skeleton::getRootBone() const
{
	return m_rootBone;
}

std::vector<Bone*> Skeleton::getChildBones(size_t boneId) const
{
	std::vector<Bone*> childBones;
	childBones.reserve(m_bones[boneId].getChildrenCount());

	for (size_t childId : m_bones[boneId].getChildren())
		childBones.push_back(const_cast<Bone*>(&m_bones[childId]));

	return childBones;
}

void Skeleton::resetPose()
{
	for (Bone& bone : m_bones)
		bone.setCurrentPoseTransform(matrix4());
}

const matrix4 & Skeleton::getGlobalInverseTransform() const
{
	return m_globalInverseTransform;
}

void Skeleton::applyPose(const SkeletonPose & pose)
{
	applyTransformsToBonesHierarchy(pose, m_rootBone, matrix4());
}

const matrix4& Skeleton::getLocalBoneTransform(size_t boneId) const
{
	return m_bones[boneId].getCurrentPoseTransform();
}

void Skeleton::applyTransformsToBonesHierarchy(const SkeletonPose& pose, Bone* bone, const matrix4& parentTransform) {
	matrix4 currentBoneTransform;

	if (!pose.isBoneAffected(bone->getId())) {
		currentBoneTransform = parentTransform * bone->getRelativeToParentSpaceTransform();
	}
	else {
		currentBoneTransform = parentTransform * pose.getBoneTransform(bone->getId());
	}

	for (size_t childId : m_bones[bone->getId()].getChildren()) {
		applyTransformsToBonesHierarchy(pose, &m_bones[childId], currentBoneTransform);
	}

	if (pose.isBoneAffected(bone->getId()))
		bone->setCurrentPoseTransform(m_globalInverseTransform * currentBoneTransform * bone->getLocalToBoneSpaceTransform());
}