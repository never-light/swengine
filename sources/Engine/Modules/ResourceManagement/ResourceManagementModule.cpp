#include "ResourceManagementModule.h"

ResourceManagementModule::ResourceManagementModule()
{

}

ResourceManager* ResourceManagementModule::getResourceManager(ResourceType type) const
{
    return m_resourceManagers.at(type).get();
}

void ResourceManagementModule::registerResourceManager(ResourceType type, ResourceManager* manager)
{
    m_resourceManagers.insert({ type, std::unique_ptr<ResourceManager>(manager) });
}
