#include "GameObjectsStore.h"

#include <Engine\assertions.h>

GameObjectsStore::GameObjectsStore()
	: m_player(nullptr),
	m_removeObjectCallback(nullptr),
	m_relocateObjectCallback(nullptr)
{
}

GameObjectsStore::~GameObjectsStore()
{
	for (GameObject* object : m_gameObjects)
		delete object;
}

void GameObjectsStore::update()
{
	for (GameObject* object : m_removedObjects)
		delete object;

	m_removedObjects.clear();
}

void GameObjectsStore::registerGameObject(GameObject * object)
{
	_assert(object != nullptr && object->getGameObjectId() == 0);

	object->setGameObjectId(m_gameObjects.size() + 1);
	m_gameObjects.push_back(object);

	if (object->isPlayer())
		m_player = static_cast<Player*>(object);

	if (m_registerObjectCallback != nullptr)
		m_registerObjectCallback(object);
}

void GameObjectsStore::removeGameObject(GameObject * object) {
	_assert(object != nullptr && object->getGameObjectId() != 0);

	m_gameObjects.erase((m_gameObjects.begin() + (object->getGameObjectId() - 1)));

	if (m_removeObjectCallback != nullptr)
		m_removeObjectCallback(object);

	m_removedObjects.push_back(object);
}

void GameObjectsStore::relocateObject(GameObject * object, GameObject::Location newLocation)
{
	_assert(object != nullptr && object->getGameObjectId() != 0);

	GameObject::Location oldLocation = object->getGameObjectLocation();

	object->setGameObjectLocation(newLocation);

	if (m_relocateObjectCallback != nullptr)
		m_relocateObjectCallback(object, oldLocation, newLocation);
}

const std::vector<GameObject*>& GameObjectsStore::getObjects() const
{
	return m_gameObjects;
}

Player * GameObjectsStore::getPlayer() const
{
	return m_player;
}

void GameObjectsStore::setRemoveObjectCallback(const RemoveObjectCallback & callback)
{
	m_removeObjectCallback = callback;
}

void GameObjectsStore::setRelocateObjectCallback(const RelocateObjectCallback & callback)
{
	m_relocateObjectCallback = callback;
}

void GameObjectsStore::setRegisterObjectCallback(const RegisterObjectCallback & callback)
{
	m_registerObjectCallback = callback;
}
