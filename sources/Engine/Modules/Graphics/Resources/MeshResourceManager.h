#pragma once

#include <optional>
#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/GraphicsSystem/Mesh.h"

struct MeshResourceConfig {
  MeshResourceConfig() = default;

  std::string resourcePath;
  std::optional<std::string> skeletonResourceId;
};

class MeshResourceManager : public ResourceManager<Mesh, MeshResourceConfig> {
 public:
  explicit MeshResourceManager(ResourcesManager* resourcesManager);
  ~MeshResourceManager() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
