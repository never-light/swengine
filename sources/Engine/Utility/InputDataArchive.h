#pragma once

#include <cereal/archives/xml.hpp>

class ResourcesManager;

class InputDataArchive : public cereal::XMLInputArchive {
 public:
  InputDataArchive(std::istream& stream, std::shared_ptr<ResourcesManager> resourcesManager);

  [[nodiscard]] ResourcesManager& getResourcesManager();

 private:
  std::shared_ptr<ResourcesManager> m_resourcesManager;
};
