#pragma once

#include "GameWorld.h"
#include "GameObjectsComponentsIterator.h"


template<class ...ComponentsTypes>
inline GameObjectsComponentsIterator<ComponentsTypes...>::GameObjectsComponentsIterator(GameWorld * world, size_t gameObjectIndex, bool isEnd)
	: m_gameWorld(world),
	m_gameObjectIndex(gameObjectIndex),
	m_isEnd(isEnd)
{
}

template<class ...ComponentsTypes>
inline GameObjectsComponentsIterator<ComponentsTypes...>::~GameObjectsComponentsIterator()
{
}

template<class ...ComponentsTypes>
inline bool GameObjectsComponentsIterator<ComponentsTypes...>::isEnd() const
{
	return m_isEnd;
}

template<class ...ComponentsTypes>
inline GameWorld * GameObjectsComponentsIterator<ComponentsTypes...>::getGameWorld() const
{
	return m_gameWorld;
}

template<class ...ComponentsTypes>
inline GameObject * GameObjectsComponentsIterator<ComponentsTypes...>::getGameObject() const
{
	if (m_isEnd)
		return nullptr;

	return m_gameWorld->getGameObjectByIndex(m_gameObjectIndex);
}

template<class ...ComponentsTypes>
inline GameObject * GameObjectsComponentsIterator<ComponentsTypes...>::operator*() const
{
	return getGameObject();
}

template<class ...ComponentsTypes>
inline bool GameObjectsComponentsIterator<ComponentsTypes...>::operator==(const GameObjectsComponentsIterator & it) const
{
	if (m_isEnd)
		return it.m_isEnd;

	return m_gameObjectIndex == it.m_gameObjectIndex;
}

template<class ...ComponentsTypes>
inline bool GameObjectsComponentsIterator<ComponentsTypes...>::operator!=(const GameObjectsComponentsIterator & it) const
{
	if (m_isEnd)
		return !it.m_isEnd; 

	return m_gameObjectIndex != it.m_gameObjectIndex;
}

template<class ...ComponentsTypes>
inline GameObjectsComponentsIterator<ComponentsTypes...>&
	GameObjectsComponentsIterator<ComponentsTypes...>::operator++() 
{
	size_t gameObjectsCount = m_gameWorld->getGameObjectsCount();

	m_gameObjectIndex++;

	while (m_gameObjectIndex < gameObjectsCount && (getGameObject()->isDestroyed() ||
        !getGameObject()->template hasComponent<ComponentsTypes...>()))
	{
		m_gameObjectIndex++;
	}

	if (m_gameObjectIndex >= gameObjectsCount)
		m_isEnd = true;

	return *this;
}
