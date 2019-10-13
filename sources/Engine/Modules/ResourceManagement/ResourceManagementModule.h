#pragma once

#include <unordered_map>
#include "ResourceManager.h"

class ResourceManagementModule final
{
public:
    ResourceManagementModule();
    ~ResourceManagementModule();

    std::shared_ptr<ResourceManager> getResourceManager() const;

private:
    std::shared_ptr<ResourceManager> m_resourceManager;
};
