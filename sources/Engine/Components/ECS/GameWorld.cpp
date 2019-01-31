#include "GameWorld.h"
#include <algorithm>

GameWorld::GameWorld()
{
}

GameWorld::~GameWorld()
{
	for (GameSystem* gameSystem : m_gameSystems)
		gameSystem->unconfigure(this);

	for (GameObject* object : m_gameObjects) {
		for (auto const&[componentTypeId, componentInstance] : object->m_components)
			delete componentInstance;

		object->m_isDestroyed = true;

		delete object;
	}
}

void GameWorld::update(float delta)
{
	for (GameSystem* system : m_gameSystems)
		system->update(this, delta);

	removeDestroyedObjects();
}

void GameWorld::render()
{
	for (GameSystem* system : m_gameSystems)
		system->render(this);
}

void GameWorld::addGameSystem(GameSystem * system)
{
	m_gameSystems.push_back(system);
	system->configure(this);
}

void GameWorld::removeGameSystem(GameSystem * system)
{
	m_gameSystems.erase(std::remove(m_gameSystems.begin(), m_gameSystems.end(), system), m_gameSystems.end());
	system->unconfigure(this);
}

GameObject * GameWorld::createGameObject()
{
	m_lastGameObjectId++;
	GameObject* gameObject = new GameObject(m_lastGameObjectId, this);

	m_gameObjects.push_back(gameObject);

	return gameObject;
}

GameObject * GameWorld::findGameObject(const std::function<bool(GameObject*)> predicate) const
{
	for (GameObject* object : m_gameObjects)
		if (!object->isDestroyed() && predicate(object))
			return object;

	return nullptr;
}

GameObject * GameWorld::getGameObjectByIndex(size_t index) const
{
	return m_gameObjects[index];
}

size_t GameWorld::getGameObjectsCount() const
{
	return m_gameObjects.size();
}

void GameWorld::removeGameObject(GameObject * gameObject)
{
	gameObject->m_isDestroyed = true;
}

void GameWorld::forEach(std::function<void(GameObject*)> action)
{
	for (GameObject* gameObject : this->all()) {
		action(gameObject);
	}
}

GameObjectsSequentialView GameWorld::all()
{
	GameObjectsSequentialIterator begin(this, 0, false);
	GameObjectsSequentialIterator end(this, m_gameObjects.size(), true);
	
	return GameObjectsSequentialView(begin, end);
}

void GameWorld::cancelEventsListening(BaseEventsListener * listener)
{
	for (auto&[typeId, listeners] : m_eventsListeners) {
		listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());
	}
}

void GameWorld::removeDestroyedObjects() {
	std::for_each(m_gameObjects.begin(), m_gameObjects.end(), [](GameObject*& obj)  {
		if (obj->isDestroyed()) {
			for (auto const&[componentTypeId, componentInstance] : obj->m_components)
				delete componentInstance;

			delete obj;
			obj = nullptr;
		}
	});

	m_gameObjects.erase(std::remove(m_gameObjects.begin(), m_gameObjects.end(), nullptr), m_gameObjects.end());
}

GameObject * GameWorld::findGameObject(GameObjectId id) const
{
	for (GameObject* object : m_gameObjects)
		if (!object->isDestroyed() && object->getId() == id)
			return object;

	return nullptr;
}
