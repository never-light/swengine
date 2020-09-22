#pragma once

#include <utility>

#include "GameObject.h"
#include "GameObjectsStorage.h"

template<class T, class ...Args>
inline GameObjectComponentHandle<T> GameObject::addComponent(Args&& ...args)
{
  SW_ASSERT(isValid() && !hasComponent<T>());

  return m_objectsStorage->assignComponent<T, Args...>(*this, std::forward<Args>(args) ...);
}

template<class T>
inline void GameObject::removeComponent()
{
  SW_ASSERT(isValid() && hasComponent<T>());

  m_objectsStorage->removeComponent<T>(*this);
}

template<class T>
inline GameObjectComponentHandle<T> GameObject::getComponent()
{
  SW_ASSERT(isValid());

  return m_objectsStorage->getComponent<T>(*this);
}

template<class T>
inline bool GameObject::hasComponent() const
{
  SW_ASSERT(isValid());

  return m_objectsStorage->hasComponent<T>(*this);
}

inline GameObjectId GameObject::getId() const
{
  SW_ASSERT(isValid());

  return m_id;
}

inline const std::string& GameObject::getName() const
{
  SW_ASSERT(isValid());

  return m_objectsStorage->m_gameObjects[m_id].name;
}

inline size_t GameObject::getRevision() const
{
  SW_ASSERT(isValid());

  return m_revision;
}

inline bool GameObject::isAlive() const
{
  return isValid();
}

inline bool GameObject::isFormed() const
{
  return m_id != GameObjectNone;
}

inline bool GameObject::isValid() const
{
  // TODO: initialize m_objectsStorage in all cases and remove m_id != GameObjectNone condition
  return isFormed() && m_objectsStorage->m_gameObjects[m_id].id != GameObjectNone &&
    m_objectsStorage->m_gameObjects[m_id].revision == m_revision;
}