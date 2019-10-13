#pragma once

#include <unordered_map>
#include <type_traits>

#include "Resource.h"
#include "ResourceInstance.h"

class ResourceManager : public std::enable_shared_from_this<ResourceManager>
{
public:
    ResourceManager();
    ~ResourceManager();

    void declareResource(const std::string& resourceId, const ResourceSource& source);

    const ResourceSource& getResourceSource(const std::string& resourceId);

    template<class T>
    std::shared_ptr<ResourceInstance> getResource(const std::string& resourceId);

private:
    std::unordered_map<std::string, ResourceSource> m_resourcesSources;
    std::unordered_map<std::string, std::shared_ptr<ResourceInstance>> m_resourcesInstances;
};

template<class T>
std::shared_ptr<ResourceInstance> ResourceManager::getResource(const std::string &resourceId)
{
    static_assert(std::is_base_of_v<Resource, T>);

    auto resourceInstanceIt = m_resourcesInstances.find(resourceId);

    if (resourceInstanceIt != m_resourcesInstances.end()) {
        return resourceInstanceIt->second;
    }

    auto resource = std::make_unique<T>();

    auto resourceInstance = std::make_shared<ResourceInstance>(resourceId, std::move(resource), shared_from_this());
    m_resourcesInstances.insert({ resourceId, resourceInstance });

    return resourceInstance;
}
