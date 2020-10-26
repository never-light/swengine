#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Audio/AudioClip.h"

struct AudioClipResourceConfig {
  AudioClipResourceConfig() = default;

  std::string resourcePath;
};

class AudioClipResource : public ResourceTypeManager<AudioClip, AudioClipResourceConfig> {
 public:
  explicit AudioClipResource(ResourcesManager* resourcesManager);
  ~AudioClipResource() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
