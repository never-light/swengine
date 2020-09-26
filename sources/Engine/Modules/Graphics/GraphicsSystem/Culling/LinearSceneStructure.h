#pragma once

#include "SceneAccelerationStructure.h"

class LinearSceneStructure : public SceneAccelerationStructure {
 public:
  LinearSceneStructure() = default;
  ~LinearSceneStructure() override = default;

  void buildFromObjectsList(std::vector<GameObject>& objects) override;

  void addObject(GameObject object) override;
  void removeObject(GameObject object) override;

  void queryNearestDynamicNeighbors(
    const glm::vec3& origin,
    float radius,
    std::vector<GameObject>& result) override;

  void queryVisibleObjects(Camera& camera, std::vector<GameObject>& result) override;

  void clear() override;

  [[nodiscard]] size_t getObjectsCount() const override;

 private:
  static void queryVisibleObjects(
    Camera& camera,
    std::vector<GameObject>& objects,
    std::vector<GameObject>& result);


 private:
  GameObject m_levelObject;

  std::vector<GameObject> m_staticObjects;
  std::vector<GameObject> m_dynamicObjects;
};
