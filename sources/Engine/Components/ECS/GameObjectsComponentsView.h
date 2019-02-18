#pragma once

#include "GameObjectsComponentsIterator.h"

/*!
 * \brief Class for viewing of the collection of game objects with specified components
 *
 * This class allows to iterate over game objects collection with specified components
 */
template<class... ComponentsTypes>
class GameObjectsComponentsView {
public:
	GameObjectsComponentsView(const GameObjectsComponentsIterator<ComponentsTypes...>& begin, 
		const GameObjectsComponentsIterator<ComponentsTypes...>& end);
	~GameObjectsComponentsView();

	/*!
	 * \brief Returns an iterator to the beginning of the collection
	 *
	 * \return beginning of the sequence iterator
	 */
	const GameObjectsComponentsIterator<ComponentsTypes...>& begin() const;

	/*!
	 * \brief Returns an iterator to the end of the collection
	 *
	 * \return end of the sequence iterator
	 */
	const GameObjectsComponentsIterator<ComponentsTypes...>& end() const;

private:
	GameObjectsComponentsIterator<ComponentsTypes...> m_begin;
	GameObjectsComponentsIterator<ComponentsTypes...> m_end;
};