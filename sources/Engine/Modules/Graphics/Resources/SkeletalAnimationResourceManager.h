#pragma once

#include <optional>
#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Graphics/GraphicsSystem/Animation/AnimationClip.h"

struct SkeletalAnimationResourceConfig {
  SkeletalAnimationResourceConfig() = default;

  std::string resourcePath;
};

class SkeletalAnimationResourceManager : public ResourceManager<AnimationClip, SkeletalAnimationResourceConfig> {
 public:
  explicit SkeletalAnimationResourceManager(ResourcesManager* resourcesManager);
  ~SkeletalAnimationResourceManager() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
