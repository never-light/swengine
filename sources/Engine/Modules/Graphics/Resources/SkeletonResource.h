#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/Resource.h"
#include "Modules/Graphics/GraphicsSystem/Animation/Skeleton.h"

struct SkeletonResourceParameters : ResourceSourceParameters {
};

class SkeletonResource : public Resource {
 public:
  using ParametersType = SkeletonResourceParameters;

 public:
  SkeletonResource();
  ~SkeletonResource() override;

  void load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) override;
  void unload() override;

  [[nodiscard]] bool isBusy() const override;

  static std::shared_ptr<Skeleton> loadFromFile(const std::string& path,
                                                const SkeletonResourceParameters& parameters);

  static ParametersType buildDeclarationParameters(const pugi::xml_node& declarationNode,
                                                   const ParametersType& defaultParameters);

 public:
  [[nodiscard]] std::shared_ptr<Skeleton> getSkeleton() const;

 private:
  std::shared_ptr<Skeleton> m_skeleton;
};
