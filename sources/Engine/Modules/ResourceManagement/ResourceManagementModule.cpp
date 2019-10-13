#include "ResourceManagementModule.h"

ResourceManagementModule::ResourceManagementModule()
    : m_resourceManager(std::make_shared<ResourceManager>())
{

}

ResourceManagementModule::~ResourceManagementModule()
{

}

std::shared_ptr<ResourceManager> ResourceManagementModule::getResourceManager() const
{
    return m_resourceManager;
}
