#pragma once

#include "BaseResourceInstance.h"

template<class T>
class ResourceInstance : public BaseResourceInstance {
public:
	ResourceInstance(T* resourceData);
	virtual ~ResourceInstance();

	T& getDataRef();
	T* getDataPtr();

protected:
	T* m_resourceData;
};

template<class T>
inline ResourceInstance<T>::ResourceInstance(T* componentData)
	: BaseResourceInstance(),
	m_resourceData(componentData)
{
}

template<class T>
inline ResourceInstance<T>::~ResourceInstance()
{
	delete m_resourceData;
}

template<class T>
inline T & ResourceInstance<T>::getDataRef()
{
	return *m_resourceData;
}

template<class T>
inline T* ResourceInstance<T>::getDataPtr()
{
	return m_resourceData;
}
