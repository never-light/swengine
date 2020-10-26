#pragma once

#include <optional>
#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Graphics/GraphicsSystem/Animation/AnimationClip.h"

struct SkeletalAnimationResourceParameters {
  SkeletalAnimationResourceParameters() = default;

  std::string resourcePath;
};

class SkeletalAnimationResource : public ResourceTypeManager<AnimationClip, SkeletalAnimationResourceParameters> {
 public:
  explicit SkeletalAnimationResource(ResourcesManager* resourcesManager);
  ~SkeletalAnimationResource() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
