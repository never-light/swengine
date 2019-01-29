#pragma once

class GameObject;
class GameWorld;

class GameObjectsSequentialIterator {
public:
	GameObjectsSequentialIterator(GameWorld* world, size_t gameObjectIndex, bool isEnd);
	~GameObjectsSequentialIterator();

	bool isEnd() const;

	GameWorld* getGameWorld() const;
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