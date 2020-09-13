#pragma once

#include "GameObjectsStorage.h"

/*!
 * \brief Iterator over game objects with specified component
 */
template<class ComponentType>
class GameObjectsComponentsIterator {
 public:
  GameObjectsComponentsIterator(GameObjectsStorage* gameObjectsStorage,
    size_t gameObjectIndex,
    bool isEnd)
    : m_gameObjectIndex(gameObjectIndex),
      m_isEnd(isEnd),
      m_gameObjectsStorage(gameObjectsStorage)
  {

  }

  ~GameObjectsComponentsIterator() = default;

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
  [[nodiscard]] GameObject getGameObject() const
  {
    if (m_isEnd || m_gameObjectIndex >= m_gameObjectsStorage->getSize()) {
      return GameObject();
    }

    GameObject gameObject = m_gameObjectsStorage->getByIndex(m_gameObjectIndex);
    return gameObject;
  }

  inline GameObject operator*() const
  {
    return getGameObject();
  }

  inline bool operator==(const GameObjectsComponentsIterator& it) const
  {
    return m_isEnd ? it.m_isEnd : m_gameObjectIndex == it.m_gameObjectIndex;
  }

  inline bool operator!=(const GameObjectsComponentsIterator& it) const
  {
    return m_isEnd ? !it.m_isEnd : m_gameObjectIndex != it.m_gameObjectIndex;
  }

  GameObjectsComponentsIterator& operator++()
  {
    size_t gameObjectsCount = m_gameObjectsStorage->getSize();

    m_gameObjectIndex++;

    while (m_gameObjectIndex < gameObjectsCount) {
      GameObject object = m_gameObjectsStorage->getByIndex(m_gameObjectIndex);

      if (!object.isAlive() || !object.hasComponent<ComponentType>()) {
        m_gameObjectIndex++;
      }
      else {
        break;
      }
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
