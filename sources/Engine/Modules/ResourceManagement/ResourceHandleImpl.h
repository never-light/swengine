#pragma once

#include "ResourceHandle.h"
#include "ResourcesManager.h"

template<class T>
ResourceHandle<T>::ResourceHandle(size_t resourceIndex, T* resourcePtr, ResourcesManager* resourcesManager)
  : m_resourceIndex(resourceIndex),
    m_resourcePtr(resourcePtr),
    m_resourcesManager(resourcesManager)
{
  if (m_resourceIndex != RESOURCE_ID_INVALID) {
    m_resourcesManager->increaseReferenceCounter(this);
  }
}

template<class T>
ResourceHandle<T>::~ResourceHandle()
{
  if (m_resourceIndex != RESOURCE_ID_INVALID) {
    m_resourcesManager->decreaseReferenceCounter(this);
  }
}

template<class T>
ResourceHandle<T>::ResourceHandle(const ResourceHandle<T>& handle)
  : m_resourceIndex(handle.m_resourceIndex),
    m_resourcePtr(handle.m_resourcePtr),
    m_resourcesManager(handle.m_resourcesManager)
{
  if (m_resourceIndex != RESOURCE_ID_INVALID) {
    m_resourcesManager->increaseReferenceCounter(this);
  }
}

template<class T>
ResourceHandle<T>::ResourceHandle(ResourceHandle<T>&& handle) noexcept
  : m_resourceIndex(handle.m_resourceIndex),
    m_resourcePtr(handle.m_resourcePtr),
    m_resourcesManager(handle.m_resourcesManager)
{
  handle.m_resourceIndex = RESOURCE_ID_INVALID;
  handle.m_resourcePtr = nullptr;
  handle.m_resourcesManager = nullptr;
}

template<class T>
ResourceHandle<T>& ResourceHandle<T>::operator=(const ResourceHandle<T>& other)
{
  if (this != &other) {
    this->m_resourcePtr = other.m_resourcePtr;
    this->m_resourceIndex = other.m_resourceIndex;
    this->m_resourcesManager = other.m_resourcesManager;

    if (this->m_resourceIndex != RESOURCE_ID_INVALID) {
      m_resourcesManager->increaseReferenceCounter(this);
    }
  }

  return *this;
}

template<class T>
ResourceHandle<T>& ResourceHandle<T>::operator=(ResourceHandle<T>&& other) noexcept
{
  if (this != &other) {
    this->m_resourcePtr = other.m_resourcePtr;
    this->m_resourceIndex = other.m_resourceIndex;
    this->m_resourcesManager = other.m_resourcesManager;

    other.m_resourceIndex = RESOURCE_ID_INVALID;
    other.m_resourcePtr = nullptr;
    other.m_resourcesManager = nullptr;
  }

  return *this;
}

template<class T>
[[nodiscard]] inline const std::string& ResourceHandle<T>::getResourceId() const
{
  SW_ASSERT(m_resourceIndex != RESOURCE_ID_INVALID);
  return m_resourcesManager->getResourceIdByIndex(m_resourceIndex);
}

template<class T>
template<class Archive>
void ResourceHandle<T>::load(Archive& archive)
{
  auto& inputArchive = dynamic_cast<InputDataArchive&>(archive);

  std::string resourceId;
  inputArchive(resourceId);

  *this = inputArchive.getResourcesManager().getResource<T>(resourceId);
}
