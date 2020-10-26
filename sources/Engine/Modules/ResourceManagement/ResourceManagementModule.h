#pragma once

#include <unordered_map>
#include <memory>

#include "ResourceManager.h"

class ResourceManagementModule final {
 public:
  ResourceManagementModule();
  ~ResourceManagementModule();

  [[nodiscard]] std::shared_ptr<ResourcesManager> getResourceManager() const;

 private:
  std::shared_ptr<ResourcesManager> m_resourceManager;
};
