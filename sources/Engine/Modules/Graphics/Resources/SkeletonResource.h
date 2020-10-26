#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Skeleton.h"

struct SkeletonResourceParameters {
  SkeletonResourceParameters() = default;

  std::string resourcePath;
};

class SkeletonResource : public ResourceTypeManager<Skeleton, SkeletonResourceParameters> {
 public:
  explicit SkeletonResource(ResourcesManager* resourcesManager);
  ~SkeletonResource() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
