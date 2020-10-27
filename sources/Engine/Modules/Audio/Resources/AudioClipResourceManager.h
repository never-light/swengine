#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Audio/AudioClip.h"

struct AudioClipResourceConfig {
  AudioClipResourceConfig() = default;

  std::string resourcePath;
};

class AudioClipResourceManager : public ResourceManager<AudioClip, AudioClipResourceConfig> {
 public:
  explicit AudioClipResourceManager(ResourcesManager* resourcesManager);
  ~AudioClipResourceManager() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
