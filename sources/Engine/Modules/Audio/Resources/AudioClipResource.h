#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/Resource.h"
#include "Modules/Audio/AudioClip.h"

struct AudioClipResourceParameters : ResourceSourceParameters {
};

class AudioClipResource : public Resource {
 public:
  using ParametersType = AudioClipResourceParameters;

 public:
  AudioClipResource();
  ~AudioClipResource() override;

  void load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) override;
  void unload() override;

  [[nodiscard]] bool isBusy() const override;

  static std::shared_ptr<AudioClip> loadFromFile(const std::string& path, const ParametersType& parameters);

  static ParametersType buildDeclarationParameters(const pugi::xml_node& declarationNode,
    const ParametersType& defaultParameters);

 public:
  [[nodiscard]] std::shared_ptr<AudioClip> getAudioClip() const;

 private:
  std::shared_ptr<AudioClip> m_audioClip;
};
