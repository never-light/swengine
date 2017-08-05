#pragma once

class SceneNode;

enum class SceneObjectType {
	Camera, Model
};

class SceneObject {
public:
	SceneObject(SceneObjectType);
	virtual ~SceneObject();

	void setParentSceneNode(SceneNode*);
	SceneNode* getParentSceneNode() const;

	SceneObjectType getSceneObjectType() const;
protected:
	SceneNode* m_parentSceneNode;
	SceneObjectType m_sceneObjectType;
};