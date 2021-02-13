#include "precompiled.h"

#pragma hdrstop

#include "ResourceManagementModule.h"

#include <spdlog/spdlog.h>

ResourceManagementModule::ResourceManagementModule()
  : m_resourceManager(std::make_shared<ResourcesManager>())
{
  spdlog::info("Initialize resource management module...");
}

ResourceManagementModule::~ResourceManagementModule() = default;

std::shared_ptr<ResourcesManager> ResourceManagementModule::getResourceManager() const
{
  return m_resourceManager;
}
