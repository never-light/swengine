#pragma once

class GameObject;
class GameWorld;

/*!
 * \brief Iterator over game objects with specifed components
 */
template<class... ComponentsTypes>
class GameObjectsComponentsIterator {
public:
	GameObjectsComponentsIterator(GameWorld* world, size_t gameObjectIndex, bool isEnd);
	~GameObjectsComponentsIterator();

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

	bool operator==(const GameObjectsComponentsIterator& it) const;
	bool operator!=(const GameObjectsComponentsIterator& it) const;

	GameObjectsComponentsIterator& operator++();

private:
	size_t m_gameObjectIndex;
	bool m_isEnd;

	GameWorld* m_gameWorld;
};
