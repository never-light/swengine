#include "SceneNode.h"

SceneNode::SceneNode()
	: m_parentSceneNode(nullptr),
	m_position(0, 0, 0),
	m_scale(1, 1, 1),
	m_orientation(glm::normalize(glm::quat()))
{

}

SceneNode::~SceneNode() {
	for (auto it = m_childSceneNodes.begin(); it != m_childSceneNodes.end(); it++) {
		delete it->second;
	}
}

void SceneNode::setName(const std::string& name) {
	m_name = name;
}

const std::string& SceneNode::getName() const {
	return m_name;
}

void SceneNode::setParentSceneNode(SceneNode* parent) {
	m_parentSceneNode = parent;
}

SceneNode* SceneNode::getParentSceneNode() const {
	return m_parentSceneNode;
}

const std::vector<SceneObject*>& SceneNode::getAttachedObjects() const {
	return m_attachedObjects;
}

void SceneNode::attachObject(SceneObject* object) {
	m_attachedObjects.push_back(object);
	object->setParentSceneNode(this);
}

void SceneNode::detachObject(SceneObject* object) {
	for (auto it = m_attachedObjects.begin(); it != m_attachedObjects.end(); it++) {
		if (object == (*it)) {
			object->setParentSceneNode(nullptr);
			m_attachedObjects.erase(it);
		}
	}
}

SceneNode* SceneNode::createChildSceneNode(const std::string& name) {
	SceneNode* childSceneNode = new SceneNode();
	childSceneNode->setName(name);
	childSceneNode->setParentSceneNode(this);

	m_childSceneNodes.insert(std::make_pair(name, childSceneNode));

	return childSceneNode;
}

SceneNode* SceneNode::getChildSceneNode(const std::string& name) const {
	return m_childSceneNodes.at(name);
}

void SceneNode::setPosition(float32 x , float32 y , float32 z) {
	m_position = vector3(x, y, z);
}

void SceneNode::setPosition(const vector3& position) {
	m_position = position;
}

void SceneNode::move(float32 x, float32 y, float32 z) {
	m_position += vector3(x, y, z);
}

void SceneNode::move(const vector3& movement) {
	m_position += movement;
}

const vector3& SceneNode::getPosition() const {
	return m_position;
}

void SceneNode::setScale(float32 x, float32 y, float32 z) {
	m_scale = vector3(x, y, z);
}

void SceneNode::setScale(const vector3& scale) {
	m_scale = scale;
}

void SceneNode::scale(float32 x, float32 y, float32 z) {
	m_scale *= vector3(x, y, z);
}

void SceneNode::scale(const vector3& scale) {
	m_scale *= scale;
}

const vector3& SceneNode::getScale() const {
	return m_scale;
}

void SceneNode::setOrientation(const quaternion& orientation) {
	m_orientation = orientation;
}

const quaternion& SceneNode::getOrientation() const {
	return m_orientation;
}

void SceneNode::rotate(const quaternion& rotation, CoordinateSystemType relativeTo) {
	if (relativeTo == CoordinateSystemType::Local) {
		m_orientation = m_orientation * glm::normalize(rotation);
	}
	else if (relativeTo == CoordinateSystemType::Parent) {
		m_orientation = glm::normalize(rotation) * m_orientation;
	}
	else if (relativeTo == CoordinateSystemType::World) {
		m_orientation = glm::normalize(rotation) * m_orientation;
	}
}

void SceneNode::rotate(const vector3& axis, float32 angle, CoordinateSystemType relativeTo ) {
	rotate(glm::angleAxis(glm::radians(angle), axis), relativeTo);
}

void SceneNode::roll(float32 angle, CoordinateSystemType relativeTo) {
	rotate(vector3(0, 0, 1), angle, relativeTo);
}

void SceneNode::pitch(float32 angle, CoordinateSystemType relativeTo) {
	rotate(vector3(1, 0, 0), angle, relativeTo);
}

void SceneNode::yaw(float32 angle, CoordinateSystemType relativeTo) {
	rotate(vector3(0, 1, 0), angle, relativeTo);
}

vector3 SceneNode::getDerivedPosition() const {
	vector3 derivedPosition = m_position;
	
	if (m_parentSceneNode != nullptr) {
		derivedPosition += m_parentSceneNode->getDerivedPosition();
	}

	return derivedPosition;
}

vector3 SceneNode::getDerivedScale() const {
	vector3 derivedScale = m_scale;

	if (m_parentSceneNode != nullptr) {
		derivedScale *= m_parentSceneNode->getDerivedScale();
	}

	return derivedScale;
}

quaternion SceneNode::getDerivedOrientation() const {
	quaternion derivedOrientation = m_orientation;

	if (m_parentSceneNode != nullptr) {
		derivedOrientation = m_parentSceneNode->getDerivedOrientation() * derivedOrientation;
	}

	return derivedOrientation;
}

matrix4 SceneNode::getTransformationMatrix() const {
	return glm::translate(matrix4(), getDerivedPosition()) * glm::mat4_cast(getDerivedOrientation()) * glm::scale(matrix4(), getDerivedScale());
}