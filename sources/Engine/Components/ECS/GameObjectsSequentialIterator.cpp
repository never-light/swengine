#include "GameObjectsSequentialIterator.h"

#include "GameWorld.h"

GameObjectsSequentialIterator::GameObjectsSequentialIterator(GameWorld * world, size_t gameObjectIndex, bool isEnd)
	: m_gameWorld(world),
	m_gameObjectIndex(gameObjectIndex),
	m_isEnd(isEnd)
{
}

GameObjectsSequentialIterator::~GameObjectsSequentialIterator()
{
}

bool GameObjectsSequentialIterator::isEnd() const
{
	return m_isEnd;
}

GameWorld * GameObjectsSequentialIterator::getGameWorld() const
{
	return m_gameWorld;
}

GameObject * GameObjectsSequentialIterator::getGameObject() const
{
	if (m_isEnd)
		return nullptr;
	
	return m_gameWorld->getGameObjectByIndex(m_gameObjectIndex);
}

GameObject * GameObjectsSequentialIterator::operator*() const
{
	return getGameObject();
}

bool GameObjectsSequentialIterator::operator==(const GameObjectsSequentialIterator & it) const
{
	if (m_isEnd)
		return it.m_isEnd;

	return m_gameObjectIndex == it.m_gameObjectIndex;
}

bool GameObjectsSequentialIterator::operator!=(const GameObjectsSequentialIterator & it) const
{
	if (m_isEnd)
		return !it.m_isEnd;

	return m_gameObjectIndex != it.m_gameObjectIndex;
}

GameObjectsSequentialIterator & GameObjectsSequentialIterator::operator++()
{
	size_t gameObjectsCount = m_gameWorld->getGameObjectsCount();

	m_gameObjectIndex++;

	while (m_gameObjectIndex < gameObjectsCount && m_gameWorld->getGameObjectByIndex(m_gameObjectIndex)->isDestroyed())
		m_gameObjectIndex++;

	if (m_gameObjectIndex >= gameObjectsCount)
		m_isEnd = true;

	
	return *this;
}
