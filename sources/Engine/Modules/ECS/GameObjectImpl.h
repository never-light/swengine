#pragma once

#include <utility>

#include "GameObject.h"
#include "GameWorld.h"
#include "swdebug.h"

template<class T, class ...Args>
inline T& GameObject::addComponent(Args&& ...args)
{
  SW_ASSERT(!hasComponent<T>());

  return m_gameWorld->assignComponent<T, Args...>(*this, std::forward<Args>(args) ...);
}

template<class T>
inline void GameObject::removeComponent()
{
  SW_ASSERT(hasComponent<T>());

  m_gameWorld->removeComponent<T>(*this);
}

template<class T>
inline T& GameObject::getComponent()
{
  auto componentIt = m_components.find(std::type_index(typeid(T)));

  SW_ASSERT(componentIt != m_components.end());

  return *std::any_cast<T>(&componentIt->second);
}

template<class T>
inline const T& GameObject::getComponent() const
{
  auto componentIt = m_components.find(std::type_index(typeid(T)));

  SW_ASSERT(componentIt != m_components.end());

  return *std::any_cast<T>(&componentIt->second);
}

template<class T>
inline bool GameObject::hasComponent() const
{
  return m_components.find(std::type_index(typeid(T))) != m_components.end();
}

template<class T, class V, class ...Types>
inline bool GameObject::hasComponent() const
{
  return hasComponent < T > () && hasComponent < V, Types...>();
}
