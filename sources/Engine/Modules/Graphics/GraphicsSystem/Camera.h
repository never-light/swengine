#pragma once

#include <memory>
#include "Transform.h"

class Camera {
public:
    Camera();
    ~Camera();

    Transform* getTransform() const;

    void setAspectRatio(float ratio);
    float getAspectRatio() const;

    void setNearClipDistance(float distance);
    float getNearClipDistance() const;

    void setFarClipDistance(float distance);
    float getFarClipDistance() const;

    void setFOVy(float fov);
    float getFOVy() const;

    glm::mat4x4 getProjectionMatrix();
    glm::mat4x4 getViewMatrix();

private:
    void resetViewCache();
    void resetProjectionCache();

private:
    // View space transform
    std::unique_ptr<Transform> m_transform;

    // Projection parameters
    float m_FOVy;
    float m_aspectRatio;
    float m_nearClipDistance;
    float m_farClipDistance;

    glm::mat4x4 m_projectionMatrixCache;
    bool m_needProjectionMatrixCacheUpdate = true;

    glm::mat4x4 m_viewMatrixCache;
};
