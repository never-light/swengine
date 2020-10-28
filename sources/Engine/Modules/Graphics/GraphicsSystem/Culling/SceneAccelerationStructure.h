#pragma once

#include <vector>
#include <span>

#include "Modules/ECS/ECS.h"
#include "Modules/Graphics/GraphicsSystem/Camera.h"

class SceneAccelerationStructure {
 public:
  SceneAccelerationStructure() = default;
  virtual ~SceneAccelerationStructure() = default;

  virtual void buildFromObjectsList(std::vector<GameObject>& objects) = 0;
  virtual void addObject(GameObject object) = 0;
  virtual void removeObject(GameObject object) = 0;

  virtual void queryNearestDynamicNeighbors(
    const glm::vec3& origin,
    float radius,
    std::vector<GameObject>& result) = 0;

  virtual void queryVisibleObjects(Camera& camera, std::vector<GameObject>& result) = 0;

  virtual void clear() = 0;

  [[nodiscard]] virtual size_t getObjectsCount() const = 0;
};
