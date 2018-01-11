#include "SceneNode.h"

SceneNode::SceneNode()
	: m_parentSceneNode(nullptr),
	m_position(0, 0, 0),
	m_scale(1, 1, 1),
	m_orientation(vector3(0.0f, 0.0f, 0.0f)),
	m_fixedYAxis(false)
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

quaternion SceneNode::getOrientation() const {
	return m_orientation;
}

void SceneNode::setOrientation(const quaternion& orientation) {
	m_orientation = orientation;
}

void SceneNode::setYawValue(real value) {
	vector3 angles = glm::eulerAngles(m_orientation);
	angles.y = glm::radians(value);

	m_orientation = quaternion(angles);
}

void SceneNode::setPitchValue(real value) {
	vector3 angles = glm::eulerAngles(m_orientation);
	angles.x = glm::radians(value);

	m_orientation = quaternion(angles);
}

void SceneNode::setRollValue(real value) {
	vector3 angles = glm::eulerAngles(m_orientation);
	angles.z = glm::radians(value);

	m_orientation = quaternion(angles);
}

void SceneNode::yaw(real angle) {
	if (m_fixedYAxis) {
		m_orientation = glm::quat(vector3(0.0, glm::radians(angle), 0.0)) * m_orientation;
	}
	else {
		m_orientation *= glm::quat(vector3(0.0, glm::radians(angle), 0.0));
	}
}

void SceneNode::pitch(real angle) {
	m_orientation *= glm::quat(vector3(glm::radians(angle), 0.0, 0.0));
}

void SceneNode::roll(real angle) {
	m_orientation *= glm::quat(vector3(0.0, 0.0, glm::radians(angle)));
}

real SceneNode::getYawValue() const {
	return glm::degrees(glm::eulerAngles(m_orientation).y);
}

real SceneNode::getPitchValue() const {
	return glm::degrees(glm::pitch(m_orientation));
}

real SceneNode::getRollValue() const {
	return glm::degrees(glm::eulerAngles(m_orientation).z);
}

void SceneNode::lookAt(const vector3& target) {
	matrix3 m;
	m[2] = glm::normalize(m_position - target);
	m[0] = glm::normalize(glm::cross(vector3(0.0f, 1.0f, 0.0f), m[2]));
	m[1] = glm::cross(m[2], m[0]);

	m_orientation = quat_cast(m);
}

void SceneNode::lookAt(real x, real y, real z) {
	lookAt(vector3(x, y, z));
}

vector3 SceneNode::getFrontDirection() const {
	return m_orientation * vector3(0.0f, 0.0f, -1.0f);
}

vector3 SceneNode::getRightDirection() const {
	return m_orientation * vector3(1.0f, 0.0f, 0.0f);
}

vector3 SceneNode::getUpDirection() const {
	return m_orientation * vector3(0.0f, 1.0f, 0.0f);
}

void SceneNode::fixYAxis(bool fixed) {
	m_fixedYAxis = fixed;
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
	return glm::translate(matrix4(), m_position) * glm::toMat4(m_orientation) * glm::scale(matrix4(), getDerivedScale());
}