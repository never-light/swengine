#pragma once

#include <cstddef>

class GameObject;
class GameWorld;

/*!
 * \brief Game objects iterator
 */
class GameObjectsSequentialIterator {
public:
	GameObjectsSequentialIterator(GameWorld* world, size_t gameObjectIndex, bool isEnd);
	~GameObjectsSequentialIterator();

	/*!
	 * \brief Checks if this is the end iterator
	 * 
	 * \return 
	 */
	bool isEnd() const;

	/*!
	 * \brief Returns the game world pointer
	 * 
	 * \return game world pointer
	 */
	GameWorld* getGameWorld() const;

	/*!
	 * \brief Returns the corresponding game object pointer
	 *
	 * \return game object pointer
	 */
	GameObject* getGameObject() const;

	GameObject* operator*() const;

	bool operator==(const GameObjectsSequentialIterator& it) const;
	bool operator!=(const GameObjectsSequentialIterator& it) const;

	GameObjectsSequentialIterator& operator++();

private:
	size_t m_gameObjectIndex;
	bool m_isEnd;

	GameWorld* m_gameWorld;
};
