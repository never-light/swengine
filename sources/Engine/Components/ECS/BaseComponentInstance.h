#pragma once

class GameObject;

class BaseComponentInstance {
public:
	BaseComponentInstance(GameObject* gameObject);
	virtual ~BaseComponentInstance();

	GameObject* getGameObject() const;

protected:
	GameObject* m_gameObject;
};