#pragma once

#include <string>
#include <memory>

#include "swdebug.h"
#include "Resource.h"

class ResourceManager;

class ResourceInstance {
 public:
  ResourceInstance(const std::string& resourceId,
                   std::unique_ptr<Resource> resource,
                   std::shared_ptr<ResourceManager> resourceManager);

  ~ResourceInstance();

  template<class T>
  T* getResource();

 private:
  void loadResource();
  void unloadResource();

 private:
  std::string m_resourceId;
  std::unique_ptr<Resource> m_resource;

  std::shared_ptr<ResourceManager> m_resourceManager;

 private:
  friend class ResourceManager;
};

template<class T>
T* ResourceInstance::getResource() {
  if (!m_resource->isLoaded()) {
    loadResource();

    SW_ASSERT(m_resource->isLoaded());
  }

  T* resource = dynamic_cast<T*>(m_resource.get());
  SW_ASSERT(resource != nullptr);

  return resource;
}

