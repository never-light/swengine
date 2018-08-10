#include "Bone.h"

Bone::Bone(size_t id, 
	const std::string & name, 
	bool isHelper,
	int parentId, 
	const std::vector<size_t>& children, 
	const glm::mat4 & localToBoneSpaceTransform,
	const glm::mat4 & relativeToParentSpaceTransform)
	: m_id(id),
	m_name(name),
	m_isHelper(isHelper),
	m_parentId(parentId),
	m_children(children),
	m_localToBoneSpaceTransform(localToBoneSpaceTransform),
	m_relativeToParentSpaceTransform(relativeToParentSpaceTransform)
{
	m_currentPoseTransform = matrix4(1.0f);
}

Bone::~Bone()
{
}

size_t Bone::getId() const
{
	return m_id;
}

std::string Bone::getName() const
{
	return m_name;
}

int Bone::getParentId() const
{
	return m_parentId;
}

bool Bone::isRoot() const
{
	return m_parentId == -1;
}

bool Bone::isHelperDummy() const
{
	return m_isHelper;
}

const matrix4& Bone::getLocalToBoneSpaceTransform() const
{
	return m_localToBoneSpaceTransform;
}

const matrix4 & Bone::getRelativeToParentSpaceTransform() const
{
	return m_relativeToParentSpaceTransform;
}

const std::vector<size_t>& Bone::getChildren() const
{
	return m_children;
}

size_t Bone::getChildrenCount() const
{
	return m_children.size();
}

const matrix4 & Bone::getCurrentPoseTransform() const
{
	return m_currentPoseTransform;
}

void Bone::setCurrentPoseTransform(const matrix4 & transform)
{
	m_currentPoseTransform = transform;
}
