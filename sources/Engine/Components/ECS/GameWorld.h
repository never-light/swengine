#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include <Engine/Utils/typeid.h>

#include "GameSystem.h"
#include "GameObject.h"
#include "ComponentInstance.h"

#include "GameObjectsSequentialView.h"
#include "GameObjectsComponentsView.h"

#include "EventsListener.h"

class GameWorld {
public:
	GameWorld();
	~GameWorld();


	void update(float delta);
	void render();

	void addGameSystem(GameSystem* system);
	void removeGameSystem(GameSystem* system);

	GameObject* createGameObject();
	GameObject* findGameObject(GameObjectId id) const;
	GameObject* findGameObject(const std::function<bool(GameObject*)> predicate) const;

	GameObject* getGameObjectByIndex(size_t index) const;

	size_t getGameObjectsCount() const;

	void removeGameObject(GameObject* gameObject);

	template<class T, class... Args>
	ComponentHandle<T> assignComponent(GameObject* gameObject, Args&&... args);

	template<class T>
	void removeComponent(GameObject* gameObject);

	void forEach(std::function<void(GameObject*)> action);

	GameObjectsSequentialView all();

	template<class... ComponentsTypes>
	GameObjectsComponentsView<ComponentsTypes...> allWith();

	template<class T>
	void subscribeEventsListener(EventsListener<T>* listener);

	template<class T>
	void unsubscribeEventsListener(EventsListener<T>* listener);

	void cancelEventsListening(BaseEventsListener* listener);

	template<class T>
	void emitEvent(const T& event);
protected:
	void removeDestroyedObjects();

protected:
	GameObjectId m_lastGameObjectId = -1;

protected:
	std::vector<GameSystem*> m_gameSystems;
	std::vector<GameObject*> m_gameObjects;

	std::unordered_map<TypeId, std::vector<BaseEventsListener*>> m_eventsListeners;
};

template<class T, class ...Args>
inline ComponentHandle<T> GameWorld::assignComponent(GameObject * gameObject, Args && ...args)
{
	TypeId typeId = getTypeId<T>();
	ComponentInstance<T>* instance = new ComponentInstance<T>(gameObject, T(std::forward<Args>(args)...));

	gameObject->m_components[typeId] = instance;

	return ComponentHandle<T>(instance->getDataPtr());
}

template<class T>
inline void GameWorld::removeComponent(GameObject * gameObject)
{
	TypeId typeId = getTypeId<T>();

	auto componentIterator = gameObject->m_components.find(typeId);

	if (componentIterator == gameObject->m_components.end())
		return;

	BaseComponentInstance* instance = componentIterator->second;

	gameObject->m_components.erase(componentIterator);

	delete instance;
}

template<class ...ComponentsTypes>
inline GameObjectsComponentsView<ComponentsTypes...> GameWorld::allWith()
{
	GameObjectsComponentsIterator<ComponentsTypes...> begin(this, 0, false);
	GameObjectsComponentsIterator<ComponentsTypes...> end(this, m_gameObjects.size(), true);

	return GameObjectsComponentsView<ComponentsTypes...>(begin, end);
}

template<class T>
inline void GameWorld::subscribeEventsListener(EventsListener<T>* listener)
{
	TypeId typeId = getTypeId<T>();

	auto eventListenersListIt = m_eventsListeners.find(typeId);

	if (eventListenersListIt == m_eventsListeners.end()) {
		m_eventsListeners.insert({ typeId, std::vector<BaseEventsListener*> { listener } });
	}
	else {
		eventListenersListIt->second.push_back(listener);
	}
}

template<class T>
inline void GameWorld::unsubscribeEventsListener(EventsListener<T>* listener)
{
	TypeId typeId = getTypeId<T>();

	auto eventListenersListIt = m_eventsListeners.find(typeId);

	if (eventListenersListIt == m_eventsListeners.end())
		return;

	eventListenersListIt->second.erase(std::remove(eventListenersListIt->second.begin(), 
		eventListenersListIt->second.end(), listener), eventListenersListIt->second.end());
}

template<class T>
inline void GameWorld::emitEvent(const T & event)
{
	TypeId typeId = getTypeId<T>();

	auto eventListenersListIt = m_eventsListeners.find(typeId);

	if (eventListenersListIt != m_eventsListeners.end()) {
		for (BaseEventsListener* baseListener : eventListenersListIt->second) {
			EventsListener<T>* listener = reinterpret_cast<EventsListener<T>*>(baseListener);
			listener->receiveEvent(this, event);
		}
	}
}
