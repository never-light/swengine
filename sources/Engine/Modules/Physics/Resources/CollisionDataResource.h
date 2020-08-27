#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/Resource.h"
#include "Modules/Physics/CollisionShapes.h"

struct CollisionDataResourceParameters : ResourceSourceParameters {
};

class CollisionDataResource : public Resource {
 public:
  using ParametersType = CollisionDataResourceParameters;

 public:
  CollisionDataResource();
  ~CollisionDataResource() override;

  void load(const ResourceDeclaration& declaration, ResourceManager& resourceManager) override;
  void unload() override;

  [[nodiscard]] bool isBusy() const override;

  static std::shared_ptr<CollisionShape> loadFromFile(const std::string& path,
    const CollisionDataResourceParameters& parameters);

  static ParametersType buildDeclarationParameters(const pugi::xml_node& declarationNode,
    const ParametersType& defaultParameters);

 public:
  [[nodiscard]] std::shared_ptr<CollisionShape> getCollisionShape() const;

 private:
  std::shared_ptr<CollisionShape> m_collisionShape;
};
