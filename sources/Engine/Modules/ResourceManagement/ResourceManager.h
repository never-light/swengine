#pragma once

#include <unordered_map>
#include <type_traits>
#include <typeindex>
#include <functional>
#include <memory>
#include <string>
#include <utility>

#include "Resource.h"

#include "Utility/xml.h"

// TODO: get rid of two-level indirection with Resource/ResourceInstance classes.
//  Store generic resource instance for each resource type. Get rid of dynamic memory
//  allocations for resources and, probably, use objects pool or even own pool for
//  each resource type.
//  Remove getResourceInstance/getResourceFromInstance methods and use single getResource
//  to get handle of a required resource. Resource instance should have reference counter
//  and handle should update it on copying and destroying. Add automatic resource unloading
//  according to references counter value.
//  Do not use resource manager as resource loader only. It should be responsible for
//  resources memory management. Store resources handles in game components instead of direct pointers
//  to meshes, textures, sounds, etc...
//  Implement lazy resources loading. Decrease references counter value on game object or game object component
//  destroying. To avoid unnecessary loading/unloading cycles for some assets, add some state flags.

#include "Utility/DynamicObjectsPool.h"
#include "Utility/TypeIdentifier.h"
#include "Utility/files.h"

#include "ResourcesStorage.h"

#include "options.h"

constexpr size_t RESOURCE_ID_INVALID = std::numeric_limits<size_t>::max();
constexpr size_t TYPE_ID_INVALID = std::numeric_limits<size_t>::max();

struct ResourceTypeIdentifierHelper {

};

using ResourceTypeIdentifier = TypeIdentifier<ResourceTypeIdentifierHelper>;

class ResourcesManager;

class ResourceState;

class Resource {
 public:
  Resource() = default;
  virtual ~Resource() = default;

  inline void setTypeId(size_t typeId)
  {
    m_typeId = typeId;
  }

  inline void setResourceId(size_t resourceId)
  {
    m_resourceId = resourceId;
  }

  [[nodiscard]] inline size_t getTypeId() const
  {
    return m_typeId;
  }

  [[nodiscard]] inline size_t getResourceId() const
  {
    return m_resourceId;
  }

 private:
  size_t m_typeId = TYPE_ID_INVALID;
  size_t m_resourceId = RESOURCE_ID_INVALID;
};

template<class T>
class ResourceHandle {
 public:
  ResourceHandle() = default;
  ResourceHandle(size_t resourceIndex, T* resourcePtr, ResourcesManager* resourcesManager);
  ResourceHandle(const ResourceHandle<T>& handle);
  ResourceHandle(ResourceHandle<T>&& handle) noexcept;

  ~ResourceHandle();

  ResourceHandle<T>& operator=(const ResourceHandle<T>& other);
  ResourceHandle<T>& operator=(ResourceHandle<T>&& other) noexcept;

  inline T* get()
  {
    return m_resourcePtr;
  }

  inline T* get() const
  {
    return m_resourcePtr;
  }

  inline T& operator*()
  {
    return *m_resourcePtr;
  }

  inline const T& operator*() const
  {
    return *m_resourcePtr;
  }

  inline T* operator->()
  {
    return m_resourcePtr;
  }

  inline T* operator->() const
  {
    return m_resourcePtr;
  }

  [[nodiscard]] inline size_t getResourceIndex() const
  {
    return m_resourceIndex;
  }

 private:

  size_t m_resourceIndex = RESOURCE_ID_INVALID;
  T* m_resourcePtr{};
  ResourcesManager* m_resourcesManager{};

 private:
  friend class ResourcesManager;
};

enum class ResourceLoadingState {
  Loaded, Unloaded
};

class ResourceState {
 public:
  explicit ResourceState(std::string resourceName)
    : m_resourceName(std::move(resourceName))
  {

  }

  virtual ~ResourceState() = default;

  inline void setLoadingState(ResourceLoadingState loadingState)
  {
    m_loadingState = loadingState;
  }

  [[nodiscard]] inline ResourceLoadingState getLoadingState() const
  {
    return m_loadingState;
  }

  [[nodiscard]] inline size_t getReferencesCount() const
  {
    return m_referencesCount;
  }

  inline void increaseReferencesCount()
  {
    m_referencesCount++;
  }

  inline void decreaseReferencesCount()
  {
    m_referencesCount--;
  }

  [[nodiscard]] inline const std::string& getResourceName() const
  {
    return m_resourceName;
  }

  [[nodiscard]] inline bool isPersistent() const
  {
    return m_isPersistent;
  }

 private:
  std::string m_resourceName;

  ResourceLoadingState m_loadingState = ResourceLoadingState::Unloaded;
  size_t m_referencesCount{};

  bool m_isPersistent{};
};

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
class ResourceTypeManagerBase : public BaseResourceManager {
 public:
  explicit ResourceTypeManagerBase(ResourcesManager* resourceManager,
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
class ResourceTypeManager : public ResourceTypeManagerBase<ResourceType> {
 public:
  explicit ResourceTypeManager(ResourcesManager* resourcesManager)
    : ResourceTypeManagerBase<ResourceType>(resourcesManager,
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

class ResourcesManager {
 public:
  ResourcesManager() = default;
  ~ResourcesManager() {
    for (const auto& resourceIt : m_resourcesNamesMap) {
      size_t resourceTypeId = resourceIt.second.first;
      size_t resourceId = resourceIt.second.second;

      ResourceState& resourceState = m_resourcesManagers[resourceTypeId]->getResourceState(resourceId);

      if constexpr (LOG_RESOURCES_MANAGEMENT) {
        if (resourceState.getReferencesCount() != 0) {
          spdlog::warn("Resource was not unloaded, {}:{}:{}", resourceTypeId, resourceId, resourceIt.first);
        }
      }
    }
  }

  template<class T>
  void registerResourceType(const std::string& configAlias,
    std::unique_ptr<BaseResourceManager> resourceManager)
  {
    size_t typeId = ResourceTypeIdentifier::getTypeId<T>();

    SW_ASSERT(typeId == m_resourcesManagers.size());

    m_resourcesManagers.emplace_back(std::move(resourceManager));
    m_resourcesTypesAliases[configAlias] = typeId;
  }

  template<class T>
  inline ResourceTypeManagerBase<T>* getResourceManager() const
  {
    size_t typeId = ResourceTypeIdentifier::getTypeId<T>();
    return dynamic_cast<ResourceTypeManagerBase<T>*>(m_resourcesManagers[typeId].get());
  }

  template<class T>
  inline ResourceHandle<T> getResource(const std::string& resourceId)
  {
    if (!m_resourcesNamesMap.contains(resourceId)) {
      spdlog::debug("Resource {} does not exists", resourceId);

      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("Resource {} does not exists", resourceId));
    }

    auto&[typeId, resourceIndex] = m_resourcesNamesMap.at(resourceId);
    LOCAL_VALUE_UNUSED(typeId);

    return getResourceManager<T>()->getResource(resourceIndex);
  }

  template<class ResourceType, class ConfigType>
  inline ConfigType* getResourceConfig(const std::string& resourceId)
  {
    auto&[typeId, resourceIndex] = m_resourcesNamesMap.at(resourceId);
    LOCAL_VALUE_UNUSED(typeId);

    return dynamic_cast<ResourceTypeManager<ResourceType, ConfigType>*>(getResourceManager<ResourceType>())
      ->getResourceConfig(resourceIndex);
  }

  template<class T>
  inline T* getResourcePtr(size_t typeId, size_t resourceIndex)
  {
    return getResourceManager<T>()->getResourcePtr(resourceIndex);
  }

  template<class T>
  inline const T* getResourcePtr(size_t typeId, size_t resourceIndex) const
  {
    return getResourceManager<T>()->getResourcePtr(resourceIndex);
  }

  template<class T>
  void increaseReferenceCounter(ResourceHandle<T>* resourceHandle)
  {
    ResourceTypeManagerBase<T>* resourceManager = getResourceManager<T>();
    ResourceState& resourceState = resourceManager->getResourceState(resourceHandle->getResourceIndex());

    if (resourceState.getReferencesCount() == 0) {
      // Resource is unloaded here, so load it

      if constexpr (LOG_RESOURCES_LOADING) {
        size_t typeId = ResourceTypeIdentifier::getTypeId<T>();
        spdlog::debug("Load resource {}:{}:{}",
          typeId,
          resourceHandle->getResourceIndex(),
          resourceState.getResourceName());
      }

      resourceManager->load(resourceHandle->getResourceIndex());
      resourceState.setLoadingState(ResourceLoadingState::Loaded);

      resourceHandle->m_resourcePtr = resourceManager->getResourcePtr(resourceHandle->getResourceIndex());
    }

    resourceState.increaseReferencesCount();
  }

  template<class T>
  void decreaseReferenceCounter(ResourceHandle<T>* resourceHandle)
  {
    ResourceTypeManagerBase<T>* resourceManager = getResourceManager<T>();
    ResourceState& resourceState = resourceManager->getResourceState(resourceHandle->getResourceIndex());

    SW_ASSERT(resourceState.getReferencesCount() > 0 &&
      resourceState.getLoadingState() == ResourceLoadingState::Loaded);

    resourceState.decreaseReferencesCount();

    if (resourceState.getReferencesCount() == 0) {
      // Resource is unused, so unload it

      if constexpr (LOG_RESOURCES_LOADING) {
        size_t typeId = ResourceTypeIdentifier::getTypeId<T>();
        spdlog::debug("Unload resource {}:{}:{}",
          typeId,
          resourceHandle->getResourceIndex(),
          resourceState.getResourceName());
      }

      resourceState.setLoadingState(ResourceLoadingState::Unloaded);
      resourceManager->freeResource(resourceHandle->getResourceIndex());
    }
  }

  void loadResourcesMap(const pugi::xml_node& resourcesNodesList)
  {
    for (pugi::xml_node declarationNode : resourcesNodesList.children("resource")) {
      std::string resourceName = declarationNode.attribute("id").as_string();

      if constexpr (LOG_RESOURCES_MANAGEMENT) {
        spdlog::debug("Load resources map item: {}", resourceName);
      }

      if (resourceName.empty()) {
        THROW_EXCEPTION(EngineRuntimeException, "Resource declaration has invalid id");
      }

      std::string resourceType = declarationNode.attribute("type").as_string();

      if (!m_resourcesTypesAliases.contains(resourceType)) {
        THROW_EXCEPTION(EngineRuntimeException, "Resource declaration has invalid type (" + resourceType + ")");
      }

      size_t typeId = m_resourcesTypesAliases[resourceType];

      BaseResourceManager* resourceManager = m_resourcesManagers[typeId].get();
      size_t resourceIndex = resourceManager->createNewResourceEntry(resourceName);

      m_resourcesNamesMap.insert({resourceName, {typeId, resourceIndex}});

      if constexpr (LOG_RESOURCES_MANAGEMENT) {
        spdlog::debug("Resource entry is created: {}:{}:{}", typeId, resourceIndex, resourceName);
      }

      resourceManager->parseConfig(resourceIndex, declarationNode);
    }
  }

  template<class ResourceType, class... Args>
  ResourceHandle<ResourceType> createResourceInPlace(Args&& ... args)
  {
    size_t typeId = ResourceTypeIdentifier::getTypeId<ResourceType>();

    std::string inPlaceResourceName = "inplace_resource_" + std::to_string(m_freeInPlaceResourceIndex);
    m_freeInPlaceResourceIndex++;

    if constexpr (LOG_RESOURCES_MANAGEMENT) {
      spdlog::debug("Create resource in-place: {}", inPlaceResourceName);
    }

    ResourceTypeManagerBase<ResourceType>* resourceManager = getResourceManager<ResourceType>();
    size_t resourceIndex = resourceManager->createNewResourceEntry(inPlaceResourceName);

    resourceManager->template allocateResource<ResourceType, Args...>(resourceIndex,
      std::forward<Args>(args)...);

    m_resourcesNamesMap.insert({inPlaceResourceName, {typeId, resourceIndex}});

    if constexpr (LOG_RESOURCES_MANAGEMENT) {
      spdlog::debug("Resource entry is created: {}:{}:{}", typeId, resourceIndex, inPlaceResourceName);
    }

    ResourceState& resourceState = resourceManager->getResourceState(resourceIndex);
    resourceState.increaseReferencesCount();
    resourceState.setLoadingState(ResourceLoadingState::Loaded);

    return ResourceHandle<ResourceType>(resourceIndex, resourceManager->getResourcePtr(resourceIndex), this);
  }

  void loadResourcesMapFile(const std::string& path)
  {
    if (!FileUtils::isFileExists(path)) {
      THROW_EXCEPTION(EngineRuntimeException, "Trying to read not existing resource map file " + path);
    }

    pugi::xml_document resourcesMap;
    pugi::xml_parse_result result = resourcesMap.load_file(path.c_str());

    if (!result) {
      THROW_EXCEPTION(EngineRuntimeException, "Trying to load resources map from invalid source");
    }

    if constexpr (LOG_RESOURCES_MANAGEMENT) {
      spdlog::debug("Load resources map from file: {}", path);
    }

    pugi::xml_node declarationsList = resourcesMap.child("resources");
    loadResourcesMap(declarationsList);
  }

  void loadResourcesMap(const std::string& content)
  {
    pugi::xml_document resourcesMap;
    pugi::xml_parse_result result = resourcesMap.load_string(content.c_str());

    if (!result) {
      THROW_EXCEPTION(EngineRuntimeException, "Trying to load resources map from invalid source");
    }

    if constexpr (LOG_RESOURCES_MANAGEMENT) {
      spdlog::debug("Load resources map from raw string");
    }

    pugi::xml_node declarationsList = resourcesMap.child("resources");
    loadResourcesMap(declarationsList);
  }

 private:
  std::vector<std::unique_ptr<BaseResourceManager>> m_resourcesManagers;

  std::unordered_map<std::string, std::pair<size_t, size_t>> m_resourcesNamesMap;
  std::unordered_map<std::string, size_t> m_resourcesTypesAliases;

  size_t m_freeInPlaceResourceIndex = 0;
};

inline ResourcesManager* BaseResourceManager::getResourceManager() const
{
  return m_resourceManager;
}

template<class T>
ResourceHandle<T>::ResourceHandle(size_t resourceIndex, T* resourcePtr, ResourcesManager* resourcesManager)
  : m_resourceIndex(resourceIndex),
    m_resourcePtr(resourcePtr),
    m_resourcesManager(resourcesManager)
{
  m_resourcesManager->increaseReferenceCounter(this);
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
  m_resourcesManager->increaseReferenceCounter(this);
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

    m_resourcesManager->increaseReferenceCounter(this);
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