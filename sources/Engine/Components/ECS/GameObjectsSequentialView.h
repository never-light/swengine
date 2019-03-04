#pragma once

#include "GameObjectsSequentialIterator.h"

/*!
 * \brief Class for game objects collection viewing
 *
 * This class allows to iterate over game objects collection
 */
class GameObjectsSequentialView {
public:
	GameObjectsSequentialView(const GameObjectsSequentialIterator& begin, const GameObjectsSequentialIterator& end);
	~GameObjectsSequentialView();

	/*!
	 * \brief Returns an iterator to the beginning of the collection 
	 * 
	 * \return beginning of the sequence iterator
	 */
	const GameObjectsSequentialIterator& begin() const;

	/*!
	 * \brief Returns an iterator to the end of the collection
	 *
	 * \return end of the sequence iterator
	 */
	const GameObjectsSequentialIterator& end() const;

private:
	GameObjectsSequentialIterator m_begin;
	GameObjectsSequentialIterator m_end;
};