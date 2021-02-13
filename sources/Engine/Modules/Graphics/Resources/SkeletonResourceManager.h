#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Skeleton.h"

struct SkeletonResourceConfig {
  SkeletonResourceConfig() = default;

  std::string resourcePath;
};

class SkeletonResourceManager : public ResourceManager<Skeleton, SkeletonResourceConfig> {
 public:
  explicit SkeletonResourceManager(ResourcesManager* resourcesManager);
  ~SkeletonResourceManager() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
