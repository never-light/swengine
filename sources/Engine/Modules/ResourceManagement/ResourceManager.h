#pragma once

#include <memory>
#include <spdlog/spdlog.h>

#include "Utility/DynamicObjectsPool.h"
#include "Utility/xml.h"

#include "swdebug.h"
#include "options.h"

#include "ResourcesStorage.h"
#include "ResourceState.h"
#include "ResourceHandle.h"
#include "Resource.h"

class ResourcesManager;

class BaseResourceManager {
 public:
  explicit BaseResourceManager(
    ResourcesManager* resourceManager,
    size_t typeId,
    std::unique_ptr<BaseResourcesStorage> resourcesStorage,
    std::unique_ptr<DynamicDataPool> configurationPool)
    : m_resourceManager(resourceManager),
      m_typeId(typeId),
      m_resourcesStorage(std::move(resourcesStorage)),
      m_configurationPool(std::move(configurationPool))
  {

  }

  virtual ~BaseResourceManager() = default;

  virtual void load(size_t resourceIndex) = 0;
  virtual void parseConfig(size_t resourceIndex, pugi::xml_node configNode) = 0;

  [[nodiscard]] size_t createNewResourceEntry(const std::string& resourceName)
  {
    size_t newResourceIndex = m_resourcesStorage->increaseStorageSize();
    LOCAL_VALUE_UNUSED(newResourceIndex);

    m_configurationPool->fit(m_configurationPool->getSize() + 1);

    m_resourcesStates.emplace_back(resourceName);

    return m_resourcesStates.size() - 1;
  }

  [[nodiscard]] inline const ResourceState& getResourceState(size_t resourceIndex) const
  {
    return m_resourcesStates[resourceIndex];
  }

  inline ResourceState& getResourceState(size_t resourceIndex)
  {
    return m_resourcesStates[resourceIndex];
  }

  inline void freeResource(size_t resourceIndex)
  {
    if constexpr (LOG_RESOURCES_MANAGEMENT) {
      auto& resourceState = getResourceState(resourceIndex);
      spdlog::debug("Free resource {}:{}:{}", m_typeId, resourceIndex, resourceState.getResourceName());
    }

    m_resourcesStorage->freeResource(resourceIndex);
  }

  [[nodiscard]] inline size_t getTypeId() const
  {
    return m_typeId;
  }

 protected:
  [[nodiscard]] inline ResourcesManager* getResourceManager() const;

 protected:
  ResourcesManager* m_resourceManager;

  size_t m_typeId;

  std::unique_ptr<BaseResourcesStorage> m_resourcesStorage;
  std::unique_ptr<DynamicDataPool> m_configurationPool;
  std::vector<ResourceState> m_resourcesStates;
};

template<class ResourceType>
class SpecificResourceManager : public BaseResourceManager {
 public:
  explicit SpecificResourceManager(ResourcesManager* resourceManager,
    std::unique_ptr<DynamicDataPool> configurationPool)
    :
    BaseResourceManager(resourceManager,
      ResourceTypeIdentifier::getTypeId<ResourceType>(),
      std::make_unique<ResourcesStorage<ResourceType>>(),
      std::move(configurationPool))
  {

  }

  template<class ResourceInheritor, class... Args>
  inline ResourceType* allocateResource(size_t resourceIndex, Args&& ... args)
  {
    SW_STATIC_ASSERT(std::is_base_of_v<Resource, ResourceType>);

    auto resourcesStorage = dynamic_cast<ResourcesStorage<ResourceType>*>(m_resourcesStorage.get());
    ResourceType* resource =
      resourcesStorage->template allocateResource<ResourceInheritor>(resourceIndex, std::forward<Args>(args)...);

    size_t typeId = ResourceTypeIdentifier::getTypeId<ResourceType>();

    resource->setTypeId(typeId);
    resource->setResourceId(resourceIndex);

    if constexpr (LOG_RESOURCES_MANAGEMENT) {
      auto& resourceState = getResourceState(resourceIndex);
      spdlog::debug("Allocate resource {}:{}:{}", typeId, resourceIndex, resourceState.getResourceName());
    }

    return resource;
  }

  inline ResourceHandle<ResourceType> getResource(size_t resourceIndex) const
  {
    SW_ASSERT(resourceIndex < m_resourcesStorage->getSize());

    return ResourceHandle<ResourceType>(resourceIndex, getResourcePtr(resourceIndex), getResourceManager());
  }

  inline ResourceType* getResourcePtr(size_t resourceIndex) const
  {
    auto resourcesStorage = dynamic_cast<ResourcesStorage<ResourceType>*>(m_resourcesStorage.get());
    return resourcesStorage->getResource(resourceIndex);
  }
};

// TODO[high]: temporary name, rename
template<class ResourceType, class ResourceConfigurationType>
class ResourceManager : public SpecificResourceManager<ResourceType> {
 public:
  explicit ResourceManager(ResourcesManager* resourcesManager)
    : SpecificResourceManager<ResourceType>(resourcesManager,
    std::make_unique<DynamicObjectsPool<ResourceConfigurationType>>(2048))
  {

  }

  template<class... Args>
  inline ResourceConfigurationType* createResourceConfig(size_t resourceIndex, Args&& ... args)
  {
    if constexpr (LOG_RESOURCES_MANAGEMENT) {
      auto& resourceState = this->getResourceState(resourceIndex);
      spdlog::debug("Allocate resource config {}:{}:{}", getTypeId(), resourceIndex, resourceState.getResourceName());
    }

    return ::new(this->m_configurationPool
      ->getObject(resourceIndex)) ResourceConfigurationType(std::forward<Args>(args)...);
  }

  inline ResourceConfigurationType* getResourceConfig(size_t resourceIndex) const
  {
    return reinterpret_cast<ResourceConfigurationType*>(this->m_configurationPool->getObject(resourceIndex));
  }

};