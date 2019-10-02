#include "GameObjectsSequentialView.h"
#include "GameObject.h"

GameObjectsSequentialView::GameObjectsSequentialView(const GameObjectsSequentialIterator & begin, const GameObjectsSequentialIterator & end)
	: m_begin(begin),
	m_end(end)
{
	// Prevent invalid iterator init
	if (m_begin.getGameObject() == nullptr || m_begin.getGameObject()->isDestroyed())
		++m_begin;
}

GameObjectsSequentialView::~GameObjectsSequentialView()
{
}

const GameObjectsSequentialIterator & GameObjectsSequentialView::begin() const
{
	return m_begin;
}

const GameObjectsSequentialIterator & GameObjectsSequentialView::end() const
{
	return m_end;
}
