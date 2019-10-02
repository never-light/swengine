#pragma once

#include "GameObject.h"
#include "GameWorld.h"

template<class T, class ...Args>
inline ComponentHandle<T> GameObject::addComponent(Args&& ...args)
{
	return m_gameWorld->assignComponent<T, Args...>(this, std::forward<Args>(args) ...);
}

template<class T>
inline void GameObject::removeComponent()
{
	m_gameWorld->removeComponent<T>(this);
}

template<class T>
inline ComponentHandle<T> GameObject::getComponent() const
{
    BaseComponentInstance* baseInstance = m_components.at(std::type_index(typeid(T)));
	return ComponentHandle<T>(reinterpret_cast<ComponentInstance<T>*>(baseInstance)->getDataPtr());
}

template<class T>
inline bool GameObject::hasComponent() const
{
    return m_components.find(std::type_index(typeid(T))) != m_components.end();
}

template<class T, class V, class ...Types>
inline bool GameObject::hasComponent() const
{
	return hasComponent<T>() && hasComponent<V, Types...>();
}
