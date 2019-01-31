#pragma once

#include <unordered_map>

#include <Engine/Utils/typeid.h>

#include "ComponentInstance.h"
#include "ComponentHandle.h"

using GameObjectId = size_t;

class GameWorld;

class GameObject {
public:
	GameObject(GameObjectId id, GameWorld* gameWorld);
	virtual ~GameObject();

	template<class T, class... Args>
	ComponentHandle<T> addComponent(Args&&... args);

	template<class T>
	void removeComponent();

	template<class T>
	ComponentHandle<T> getComponent() const;

	template<class T>
	bool hasComponent() const;

	template<class T, class V, class... Types>
	bool hasComponent() const;

	GameObjectId getId() const;

	bool isDestroyed() const;
protected:
	GameObjectId m_id;
	std::unordered_map<TypeId, BaseComponentInstance*> m_components;

	bool m_isDestroyed;

	GameWorld* m_gameWorld;

protected:
	friend class GameWorld;
};