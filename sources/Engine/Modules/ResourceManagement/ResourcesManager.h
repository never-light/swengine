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

#include "Utility/DynamicObjectsPool.h"
#include "Utility/TypeIdentifier.h"
#include "Utility/files.h"

#include "ResourcesStorage.h"
#include "ResourceManager.h"

#include "options.h"

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
  inline SpecificResourceManager<T>* getResourceManager() const
  {
    size_t typeId = ResourceTypeIdentifier::getTypeId<T>();
    return dynamic_cast<SpecificResourceManager<T>*>(m_resourcesManagers[typeId].get());
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

    return dynamic_cast<ResourceManager<ResourceType, ConfigType>*>(getResourceManager<ResourceType>())
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
    SpecificResourceManager<T>* resourceManager = getResourceManager<T>();
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
    SpecificResourceManager<T>* resourceManager = getResourceManager<T>();
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

    SpecificResourceManager<ResourceType>* resourceManager = getResourceManager<ResourceType>();
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
