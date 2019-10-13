#include "ResourceManager.h"

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

const ResourceSource& ResourceManager::getResourceSource(const std::string &resourceId)
{
    return m_resourcesSources.at(resourceId);
}

std::shared_ptr<ResourceInstance> ResourceManager::getResourceInstance(const std::string &resourceId)
{
    auto resourceInstanceIt = m_resourcesInstances.find(resourceId);

    if (resourceInstanceIt != m_resourcesInstances.end()) {
        return resourceInstanceIt->second;
    }

    const auto& typeIndex = m_resourcesTypesIds.at(resourceId);
    auto resource = std::unique_ptr<Resource>(m_resourcesFactories.at(typeIndex)());

    auto resourceInstance = std::make_shared<ResourceInstance>(resourceId, std::move(resource), shared_from_this());
    m_resourcesInstances.insert({ resourceId, resourceInstance });

    return resourceInstance;
}
