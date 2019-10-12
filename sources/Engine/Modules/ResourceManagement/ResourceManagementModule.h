#pragma once

#include <unordered_map>
#include "ResourceManager.h"

class ResourceManagementModule final
{
public:
    ResourceManagementModule();
    ~ResourceManagementModule();

    ResourceManager* getResourceManager(ResourceType type) const;

private:
    void registerResourceManager(ResourceType type, ResourceManager* manager);

private:
    std::unordered_map<ResourceType, std::unique_ptr<ResourceManager>> m_resourceManagers;
};
