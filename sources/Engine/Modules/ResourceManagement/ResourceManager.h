#pragma once

#include <unordered_map>
#include <type_traits>
#include <typeindex>
#include <functional>

#include "Resource.h"
#include "ResourceInstance.h"

class ResourceManager : public std::enable_shared_from_this<ResourceManager>
{
public:
    ResourceManager();
    ~ResourceManager();

    template <class T>
    void declareResourceType();

    template <class T>
    void declareResource(const std::string& resourceId, const ResourceSource& source);

    const ResourceSource& getResourceSource(const std::string& resourceId);
    std::shared_ptr<ResourceInstance> getResourceInstance(const std::string& resourceId);

private:
    std::unordered_map<std::string, ResourceSource> m_resourcesSources;
    std::unordered_map<std::string, std::shared_ptr<ResourceInstance>> m_resourcesInstances;

    std::unordered_map<std::type_index, std::function<Resource* ()>> m_resourcesFactories;
    std::unordered_map<std::string, std::type_index> m_resourcesTypesIds;
};

template<class T>
void ResourceManager::declareResourceType()
{
    static_assert (std::is_base_of_v<Resource, T>);

    m_resourcesFactories.insert({ std::type_index(typeid(T)), [] () {
        return new T();
    }});
}

template<class T>
void ResourceManager::declareResource(const std::string &resourceId, const ResourceSource &source)
{
    static_assert (std::is_base_of_v<Resource, T>);

    m_resourcesSources.insert({ resourceId, source });
    m_resourcesTypesIds.insert({ resourceId, std::type_index(typeid (T)) });
}
