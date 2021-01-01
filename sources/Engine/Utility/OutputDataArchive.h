#pragma once

#include <cereal/archives/xml.hpp>

class ResourcesManager;

class OutputDataArchive : public cereal::XMLOutputArchive {
 public:
  OutputDataArchive(std::ostream& stream, std::shared_ptr<ResourcesManager> resourcesManager);

  [[nodiscard]] ResourcesManager& getResourcesManager();

 private:
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};
