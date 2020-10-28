#pragma once

#include "Culling/SceneAccelerationStructure.h"

#include "FrameStats.h"

struct ObjectSceneNodeComponent {
 public:
  explicit ObjectSceneNodeComponent(bool isDrawable)
    : m_isDrawable(isDrawable)
  {

  }

  [[nodiscard]] inline bool isDrawable() const
  {
    return m_isDrawable;
  }

  void setDrawable(bool isDrawable) {
    m_isDrawable = isDrawable;
  }

 private:
  bool m_isDrawable{};
};

class GraphicsScene {
 public:
  GraphicsScene();

  void buildFromObjectsList(std::vector<GameObject>& objects);

  void addObject(GameObject object);
  void removeObject(GameObject object);

  void queryNearestDynamicNeighbors(
    const glm::vec3& origin,
    float radius,
    std::vector<GameObject>& result);

  void queryVisibleObjects(Camera& camera, std::vector<GameObject>& result);
  void queryVisibleObjects(std::vector<GameObject>& result);

  void clearObjects();

  [[nodiscard]] size_t getObjectsCount() const;
  [[nodiscard]] size_t getDrawableObjectsCount() const;

  void setActiveCamera(std::shared_ptr<Camera> camera);
  [[nodiscard]] std::shared_ptr<Camera> getActiveCamera() const;

  [[nodiscard]] const FrameStats& getFrameStats() const;
  FrameStats& getFrameStats();

 private:
  void addSceneNodeComponent(GameObject& object);

  [[nodiscard]] static bool isObjectDrawable(GameObject& object);

 private:
  std::unique_ptr<SceneAccelerationStructure> m_accelerationStructure;
  std::shared_ptr<Camera> m_activeCamera;

  size_t m_drawableObjectsCount{};

  FrameStats m_frameStats;
};
