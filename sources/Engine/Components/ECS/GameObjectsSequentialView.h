#pragma once

#include "GameObjectsSequentialIterator.h"

class GameObjectsSequentialView {
public:
	GameObjectsSequentialView(const GameObjectsSequentialIterator& begin, const GameObjectsSequentialIterator& end);
	~GameObjectsSequentialView();

	const GameObjectsSequentialIterator& begin() const;
	const GameObjectsSequentialIterator& end() const;

private:
	GameObjectsSequentialIterator m_begin;
	GameObjectsSequentialIterator m_end;
};