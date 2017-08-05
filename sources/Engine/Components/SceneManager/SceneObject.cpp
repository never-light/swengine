#include "SceneObject.h"

SceneObject::SceneObject(SceneObjectType type) : m_parentSceneNode(nullptr), m_sceneObjectType(type) {

}

SceneObject::~SceneObject() {

}

void SceneObject::setParentSceneNode(SceneNode* parent) {
	m_parentSceneNode = parent;
}

SceneNode* SceneObject::getParentSceneNode() const {
	return m_parentSceneNode;
}

SceneObjectType SceneObject::getSceneObjectType() const {
	return m_sceneObjectType;
}