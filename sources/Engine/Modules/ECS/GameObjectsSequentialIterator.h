#pragma once

#include <cstddef>
#include <utility>

#include "GameObjectsStorage.h"

class GameObject;

class GameWorld;

/*!
 * \brief Game objects iterator
 */
class GameObjectsSequentialIterator {
 public:
  GameObjectsSequentialIterator(GameObjectsStorage* gameObjectsStorage,
    size_t gameObjectIndex,
    bool isEnd)
    : m_gameObjectIndex(gameObjectIndex),
      m_isEnd(isEnd),
      m_gameObjectsStorage(gameObjectsStorage)
  {

  }

  ~GameObjectsSequentialIterator() = default;

  /*!
   * \brief Checks if this is the end iterator
   *
   * \return
   */
  [[nodiscard]] inline bool isEnd() const
  {
    return m_isEnd;
  }

  /*!
   * \brief Returns the corresponding game object pointer
   *
   * \return game object pointer
   */
  [[nodiscard]] inline GameObject getGameObject() const
  {
    if (m_isEnd) {
      return GameObject();
    }

    GameObject gameObject = m_gameObjectsStorage->getByIndex(m_gameObjectIndex);
    return gameObject;
  }

  GameObject operator*() const
  {
    return getGameObject();
  }

  bool operator==(const GameObjectsSequentialIterator& it) const
  {
    return m_isEnd ? it.m_isEnd : m_gameObjectIndex == it.m_gameObjectIndex;
  }

  bool operator!=(const GameObjectsSequentialIterator& it) const
  {
    return m_isEnd ? !it.m_isEnd : m_gameObjectIndex != it.m_gameObjectIndex;
  }

  GameObjectsSequentialIterator& operator++()
  {
    size_t gameObjectsCount = m_gameObjectsStorage->getSize();

    m_gameObjectIndex++;

    while (m_gameObjectIndex < gameObjectsCount
      && !m_gameObjectsStorage->getByIndex(m_gameObjectIndex).isAlive()) {
      m_gameObjectIndex++;
    }

    if (m_gameObjectIndex >= gameObjectsCount) {
      m_isEnd = true;
    }

    return *this;
  }

 private:
  size_t m_gameObjectIndex;
  bool m_isEnd;

  GameObjectsStorage* m_gameObjectsStorage;
};
