#pragma once

#include "Resource.h"

template<class T>
class HoldingResource : public Resource {
public:
	HoldingResource(T* holdedResource);
	virtual ~HoldingResource();

	T* getHoldedResource() const;
private:
	T * m_holdedResource;
};

template<class T>
inline HoldingResource<T>::HoldingResource(T * holdedResource) 
	: m_holdedResource(holdedResource)
{
}

template<class T>
inline HoldingResource<T>::~HoldingResource()
{
	delete m_holdedResource;
}

template<class T>
inline T * HoldingResource<T>::getHoldedResource() const
{
	return m_holdedResource;
}
