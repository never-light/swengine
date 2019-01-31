#pragma once

#include "ResourceInstance.h"

template<class T>
class ResourceHandle {
public:
	ResourceHandle()
		: m_resourcePtr(nullptr)
	{ }

	ResourceHandle(ResourceInstance<T>* resource)
		: m_resourcePtr(resource)
	{ }

	T* operator->() const {
		return m_resourcePtr->getDataPtr();
	}

	T* getRawPtr() const {
		return m_resourcePtr->getDataPtr();
	}

	bool isValid() const {
		return m_resourcePtr != nullptr;
	}

private:
	ResourceInstance<T>* m_resourcePtr;
};
