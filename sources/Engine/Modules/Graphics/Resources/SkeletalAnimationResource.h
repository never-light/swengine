#pragma once

#include <optional>
#include <string>
#include <memory>

#include "Modules/ResourceManagement/Resource.h"
#include "Modules/Graphics/GraphicsSystem/Animation/SkeletalAnimationClip.h"

struct SkeletalAnimationResourceParameters : ResourceSourceParameters {
};

class SkeletalAnimationResource : public Resource {
 public:
  using ParametersType = SkeletalAnimationResourceParameters;

 public:
  SkeletalAnimationResource();
  ~SkeletalAnimationResource() override;

  void load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) override;
  void unload() override;

  [[nodiscard]] bool isBusy() const override;

  static std::shared_ptr<SkeletalAnimationClip> loadFromFile(const std::string& path,
                                                             const SkeletalAnimationResourceParameters& parameters);

  static ParametersType buildDeclarationParameters(const pugi::xml_node& declarationNode,
                                                   const ParametersType& defaultParameters);

 public:
  [[nodiscard]] std::shared_ptr<SkeletalAnimationClip> getClip() const;

 private:
  std::shared_ptr<SkeletalAnimationClip> m_clip;
};
