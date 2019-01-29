#pragma once

#include "BaseComponentInstance.h"

template<class T>
class ComponentInstance : public BaseComponentInstance {
public:
	ComponentInstance(GameObject* gameObject, const T& componentData);
	virtual ~ComponentInstance();

	T& getDataRef();
	T* getDataPtr();

protected:
	T m_componentData;
};

template<class T>
inline ComponentInstance<T>::ComponentInstance(GameObject* gameObject, const T & componentData)
	: BaseComponentInstance(gameObject),
	m_componentData(componentData)
{
}

template<class T>
inline ComponentInstance<T>::~ComponentInstance()
{
}

template<class T>
inline T & ComponentInstance<T>::getDataRef()
{
	return m_componentData;
}

template<class T>
inline T * ComponentInstance<T>::getDataPtr()
{
	return &m_componentData;
}
