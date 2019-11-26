#include "Camera.h"

Camera::Camera()
    : m_transform(new Transform())
{

}

Camera::~Camera() {

}

Transform* Camera::getTransform() const {
    return m_transform.get();
}

void Camera::setAspectRatio(float ratio) {
    m_aspectRatio = ratio;

    resetProjectionCache();
}

float Camera::getAspectRatio() const {
    return m_aspectRatio;
}

void Camera::setNearClipDistance(float distance) {
    m_nearClipDistance = distance;

    resetProjectionCache();
}

float Camera::getNearClipDistance() const {
    return m_nearClipDistance;
}

void Camera::setFarClipDistance(float distance) {
    m_farClipDistance = distance;

    resetProjectionCache();
}

float Camera::getFarClipDistance() const {
    return m_farClipDistance;
}

void Camera::setFOVy(float fov) {
    m_FOVy = fov;

    resetProjectionCache();
}

float Camera::getFOVy() const {
    return m_FOVy;
}

glm::mat4x4 Camera::getProjectionMatrix() {
    if (m_needProjectionMatrixCacheUpdate) {
        m_projectionMatrixCache = glm::perspective(m_FOVy, m_aspectRatio, m_nearClipDistance, m_farClipDistance);

        m_needProjectionMatrixCacheUpdate = false;
    }

    return m_projectionMatrixCache;
}

glm::mat4x4 Camera::getViewMatrix()
{
    if (m_transform->isCacheOutdated()) {
        m_viewMatrixCache = glm::lookAt(m_transform->getPosition(),
            m_transform->getPosition() + m_transform->getFrontDirection(),
            m_transform->getUpDirection());

        // Trigger transformation cache update
        (void)m_transform->getTransformationMatrix();
    }

    return m_viewMatrixCache;
}

void Camera::resetProjectionCache()
{
    m_needProjectionMatrixCacheUpdate = true;

}
