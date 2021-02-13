#pragma once

#include <string>
#include <memory>

#include <glm/vec3.hpp>

#include "Modules/ResourceManagement/ResourcesManagement.h"
#include "Modules/Physics/CollisionShapes.h"

struct CollisionShapeResourceConfig {
  CollisionShapeResourceConfig() = default;

  std::string resourcePath;
};

class CollisionShapeResourceManager : public ResourceManager<CollisionShape, CollisionShapeResourceConfig> {
 public:
  explicit CollisionShapeResourceManager(ResourcesManager* resourcesManager);
  ~CollisionShapeResourceManager() override;

  void load(size_t resourceIndex) override;
  void parseConfig(size_t resourceIndex, pugi::xml_node configNode) override;
};
