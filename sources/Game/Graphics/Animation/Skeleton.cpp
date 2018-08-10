#include "Skeleton.h"

#include <Engine\Exceptions\EngineException.h>

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
