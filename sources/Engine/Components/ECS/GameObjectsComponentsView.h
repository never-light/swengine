#pragma once

#include "GameObjectsComponentsIterator.h"

template<class... ComponentsTypes>
class GameObjectsComponentsView {
public:
	GameObjectsComponentsView(const GameObjectsComponentsIterator<ComponentsTypes...>& begin, 
		const GameObjectsComponentsIterator<ComponentsTypes...>& end);
	~GameObjectsComponentsView();

	const GameObjectsComponentsIterator<ComponentsTypes...>& begin() const;
	const GameObjectsComponentsIterator<ComponentsTypes...>& end() const;

private:
	GameObjectsComponentsIterator<ComponentsTypes...> m_begin;
	GameObjectsComponentsIterator<ComponentsTypes...> m_end;
};