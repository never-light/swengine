#pragma once

#include <Game\GameObject.h>
#include <Game\Player.h>

#include <functional>

class GameObjectsStore {
public:
	using RemoveObjectCallback = std::function<void(GameObject*)>;
	using RelocateObjectCallback = std::function<void(GameObject*, GameObject::Location, GameObject::Location)>;
	using RegisterObjectCallback = std::function<void(GameObject*)>;

public:
	GameObjectsStore();
	~GameObjectsStore();

	void registerGameObject(GameObject* object);
	void removeGameObject(GameObject* object);

	void relocateObject(GameObject* object, GameObject::Location newLocation);

	const std::vector<GameObject*>& getObjects() const;
	Player* getPlayer() const;

	void setRemoveObjectCallback(const RemoveObjectCallback& callback);
	void setRelocateObjectCallback(const RelocateObjectCallback& callback);
	void setRegisterObjectCallback(const RegisterObjectCallback& callback);
protected:
	std::vector<GameObject*> m_gameObjects;

	Player* m_player;
	RemoveObjectCallback m_removeObjectCallback;
	RelocateObjectCallback m_relocateObjectCallback;
	RegisterObjectCallback m_registerObjectCallback;
};