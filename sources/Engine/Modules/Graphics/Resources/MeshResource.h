#pragma once

#include <optional>
#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/GraphicsSystem/Mesh.h"

struct MeshResourceParameters {
  MeshResourceParameters() = default;

  std::string resourcePath;
  std::optional<std::string> skeletonResourceId;
};

class MeshResource : public ResourceTypeManager<Mesh, MeshResourceParameters> {
 public:
  explicit MeshResource(ResourcesManager* resourcesManager);
  ~MeshResource() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
