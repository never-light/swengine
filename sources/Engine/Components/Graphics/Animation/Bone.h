#pragma once

#include <string>
#include <vector>

#include <Engine/Components/Math/types.h>

class Bone {
public:
	Bone(size_t id, 
		const std::string& name, 
		bool isHelper,
		int parentId, 
		const std::vector<size_t>& children, 
		const glm::mat4& localToBoneSpaceTransform,
		const glm::mat4& relativeToParentSpaceTransform);
	~Bone();

	size_t getId() const;
	std::string getName() const;

	int getParentId() const;
	bool isRoot() const;

	bool isHelperDummy() const; 

	const std::vector<size_t>& getChildren() const;
	size_t getChildrenCount() const;

	const matrix4& getLocalToBoneSpaceTransform() const;
	const matrix4& getRelativeToParentSpaceTransform() const;

	const matrix4& getCurrentPoseTransform() const;
	void setCurrentPoseTransform(const matrix4& transform);
private:
	size_t m_id;
	std::string m_name;

	bool m_isHelper;

	int m_parentId;

	std::vector<size_t> m_children;
	matrix4 m_localToBoneSpaceTransform;
	matrix4 m_relativeToParentSpaceTransform;

	matrix4 m_currentPoseTransform;
};