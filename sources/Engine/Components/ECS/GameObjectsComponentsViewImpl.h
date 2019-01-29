#pragma once

#include "GameObjectsComponentsView.h"
#include "GameWorld.h"

template<class ...ComponentsTypes>
inline GameObjectsComponentsView<ComponentsTypes...>
	::GameObjectsComponentsView(const GameObjectsComponentsIterator<ComponentsTypes...> & begin, 
		const GameObjectsComponentsIterator<ComponentsTypes...> & end)
	: m_begin(begin),
	m_end(end)
{
	GameObject* gameObject = m_begin.getGameObject();

	// Prevent invalid iterator init
	if (gameObject == nullptr || gameObject->isDestroyed() || !gameObject->hasComponent<ComponentsTypes...>())
		++m_begin;
}

template<class ...Types>
inline GameObjectsComponentsView<Types...>::~GameObjectsComponentsView()
{
}

template<class ...Types>
inline const GameObjectsComponentsIterator<Types...> & GameObjectsComponentsView<Types...>::begin() const
{
	return m_begin;
}

template<class ...Types>
inline const GameObjectsComponentsIterator<Types...> & GameObjectsComponentsView<Types...>::end() const
{
	return m_end;
}
