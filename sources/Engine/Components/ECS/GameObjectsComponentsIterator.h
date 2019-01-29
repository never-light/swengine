#pragma once

class GameObject;
class GameWorld;

template<class... ComponentsTypes>
class GameObjectsComponentsIterator {
public:
	GameObjectsComponentsIterator(GameWorld* world, size_t gameObjectIndex, bool isEnd);
	~GameObjectsComponentsIterator();

	bool isEnd() const;

	GameWorld* getGameWorld() const;
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
