#include "precompiled.h"
#pragma hdrstop

#include "ResourceManagementModule.h"

#include <spdlog/spdlog.h>

ResourceManagementModule::ResourceManagementModule()
    : m_resourceManager(std::make_shared<ResourceManager>())
{
    spdlog::info("Initialize resource management module...");
}

ResourceManagementModule::~ResourceManagementModule()
{

}

std::shared_ptr<ResourceManager> ResourceManagementModule::getResourceManager() const
{
    return m_resourceManager;
}
