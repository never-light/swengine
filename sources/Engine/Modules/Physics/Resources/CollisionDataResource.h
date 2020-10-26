#pragma once

#include <string>
#include <memory>

#include "Modules/ResourceManagement/ResourceManager.h"
#include "Modules/Physics/CollisionShapes.h"

struct CollisionDataResourceConfig {
  CollisionDataResourceConfig() = default;

  std::string resourcePath;
};

class CollisionDataResource : public ResourceTypeManager<CollisionShape, CollisionDataResourceConfig> {
 public:
  explicit CollisionDataResource(ResourcesManager* resourcesManager);
  ~CollisionDataResource() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
