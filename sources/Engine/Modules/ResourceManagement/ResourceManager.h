#pragma once

#include <unordered_map>
#include <type_traits>
#include <typeindex>
#include <functional>
#include <memory>
#include <string>

#include "Resource.h"
#include "ResourceInstance.h"

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


class ResourceManager : public std::enable_shared_from_this<ResourceManager> {
 public:
  ResourceManager();
  ResourceManager(const ResourceManager& resourceManager) = delete;
  ResourceManager(ResourceManager&& resourceManager) = delete;
  ~ResourceManager();

  template<class T>
  void declareResourceType();

  template<class T>
  void declareResourceType(const std::string& mapAlias);

  template<class T>
  void declareResouceMapAlias(const std::string& alias);

  template<class T>
  void declareResource(const std::string& resourceId, const ResourceDeclaration& declaration);

  template<class T>
  [[nodiscard]] T* getResourceFromInstance(const std::string& resourceId);

  [[nodiscard]] const ResourceDeclaration& getResourceDeclaration(const std::string& resourceId);
  [[nodiscard]] std::shared_ptr<ResourceInstance> getResourceInstance(const std::string& resourceId);

  void loadResourcesMap(const std::string& content);
  void loadResourcesMapFile(const std::string& path);

 private:
  void loadResourcesMap(const pugi::xml_node& declarationsList);

 private:
  std::unordered_map<std::string, ResourceDeclaration> m_resourcesDeclarations;
  std::unordered_map<std::string, std::shared_ptr<ResourceInstance>> m_resourcesInstances;

  std::unordered_map<std::type_index, std::function<Resource*()>> m_resourcesFactories;
  std::unordered_map<std::string, std::type_index> m_resourcesTypesIds;

  std::unordered_map<std::string,
                     std::function<void(const std::string&,
                       const ResourceSource&,
                       const pugi::xml_node&,
                       const ResourceDeclaration*)>> m_resourcesDeclarers;
};

template<class T>
T* ResourceManager::getResourceFromInstance(const std::string& resourceId)
{
  return getResourceInstance(resourceId)->getResource<T>();
}

template<class T>
void ResourceManager::declareResourceType(const std::string& mapAlias)
{
  static_assert(std::is_base_of_v<Resource, T>);

  declareResourceType<T>();
  declareResouceMapAlias<T>(mapAlias);
}

template<class T>
void ResourceManager::declareResouceMapAlias(const std::string& alias)
{
  static_assert(std::is_base_of_v<Resource, T>);

  std::function declarer = [=](const std::string& resourceId, const ResourceSource& source,
    const pugi::xml_node& declarationNode, const ResourceDeclaration* parentDeclaration) {
    using ParametersType = typename T::ParametersType;

    ParametersType defaultParametersInstance;

    if (parentDeclaration != nullptr) {
      defaultParametersInstance = std::any_cast<ParametersType>(parentDeclaration->parameters);
    }

    auto resourceParameters = T::buildDeclarationParameters(declarationNode, defaultParametersInstance);
    declareResource<T>(resourceId, ResourceDeclaration{source, resourceParameters});
  };

  m_resourcesDeclarers.insert({alias, declarer});
}

template<class T>
void ResourceManager::declareResourceType()
{
  static_assert(std::is_base_of_v<Resource, T>);

  m_resourcesFactories.insert({std::type_index(typeid(T)), []() {
    return new T();
  }});
}

template<class T>
void ResourceManager::declareResource(const std::string& resourceId, const ResourceDeclaration& declaration)
{
  static_assert(std::is_base_of_v<Resource, T>);

  m_resourcesDeclarations.insert({resourceId, declaration});
  m_resourcesTypesIds.insert({resourceId, std::type_index(typeid(T))});
}
