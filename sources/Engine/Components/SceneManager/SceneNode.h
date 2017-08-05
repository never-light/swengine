#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "SceneObject.h"

class SceneNode {
public:
	SceneNode();
	~SceneNode();

	void setName(const std::string&);
	const std::string& getName() const;

	void setParentSceneNode(SceneNode*);
	SceneNode* getParentSceneNode() const;

	const std::vector<SceneObject*>& getAttachedObjects() const;
	void attachObject(SceneObject*);
	void detachObject(SceneObject*);

	SceneNode* createChildSceneNode(const std::string&);
	SceneNode* getChildSceneNode(const std::string&) const;

	void setPosition(float32, float32, float32);
	void setPosition(const vector3&);
	void move(float32, float32 , float32);
	void move(const vector3&);
	const vector3& getPosition() const;

	void setScale(float32, float32, float32);
	void setScale(const vector3&);
	void scale(float32, float32, float32);
	void scale(const vector3&);
	const vector3& getScale() const;

	void setOrientation(const quaternion&);
	const quaternion& getOrientation() const;
	void rotate(const quaternion&, CoordinateSystemType relativeTo = CoordinateSystemType::Local);
	void rotate(const vector3&, float32, CoordinateSystemType relativeTo = CoordinateSystemType::Local);
	void roll(float32, CoordinateSystemType relativeTo = CoordinateSystemType::Local);
	void pitch(float32, CoordinateSystemType relativeTo = CoordinateSystemType::Local);
	void yaw(float32, CoordinateSystemType relativeTo = CoordinateSystemType::Local);

	vector3 getDerivedPosition() const;
	vector3 getDerivedScale() const;
	quaternion getDerivedOrientation() const;
	matrix4 getTransformationMatrix() const;
private:
	std::string m_name;
	vector3 m_position;
	vector3 m_scale;
	quaternion m_orientation;

	SceneNode* m_parentSceneNode;
	std::unordered_map<std::string, SceneNode*> m_childSceneNodes;

	std::vector<SceneObject*> m_attachedObjects;
};