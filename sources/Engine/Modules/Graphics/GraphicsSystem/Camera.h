#pragma once

#include <memory>

#include "Modules/Math/geometry.h"
#include "Transform.h"

class Camera {
 public:
  Camera();
  ~Camera();

  [[nodiscard]] Transform* getTransform() const;

  [[nodiscard]] const Frustum& getFrustum();

  void setAspectRatio(float ratio);
  [[nodiscard]] float getAspectRatio() const;

  void setNearClipDistance(float distance);
  [[nodiscard]] float getNearClipDistance() const;

  void setFarClipDistance(float distance);
  [[nodiscard]] float getFarClipDistance() const;

  void setFOVy(float fov);
  [[nodiscard]] float getFOVy() const;

  [[nodiscard]] glm::mat4x4 getProjectionMatrix();
  [[nodiscard]] glm::mat4x4 getViewMatrix();

 private:
  void resetViewCache();
  void resetProjectionCache();

 private:
  // View space transform
  std::unique_ptr<Transform> m_transform;

  Frustum m_frustum;
  bool m_needFrustumUpdate = true;

  // Projection parameters
  float m_FOVy;
  float m_aspectRatio;
  float m_nearClipDistance;
  float m_farClipDistance;

  glm::mat4x4 m_projectionMatrixCache;
  bool m_needProjectionMatrixCacheUpdate = true;

  glm::mat4x4 m_viewMatrixCache;
};
