#include "precompiled.h"

#pragma hdrstop

#include "Camera.h"

Camera::Camera()
    : m_transform(new Transform())
{

}

Camera::~Camera()
{

}

Transform* Camera::getTransform() const
{
  return m_transform.get();
}

const Frustum& Camera::getFrustum()
{
  if (m_needFrustumUpdate || m_transform->isCacheOutdated()) {
    m_frustum = Frustum::extractFromViewProjection(getViewMatrix(), getProjectionMatrix());

    m_needFrustumUpdate = false;
  }

  return m_frustum;
}

void Camera::setAspectRatio(float ratio)
{
  m_aspectRatio = ratio;

  resetProjectionCache();
}

float Camera::getAspectRatio() const
{
  return m_aspectRatio;
}

void Camera::setNearClipDistance(float distance)
{
  m_nearClipDistance = distance;

  resetProjectionCache();
}

float Camera::getNearClipDistance() const
{
  return m_nearClipDistance;
}

void Camera::setFarClipDistance(float distance)
{
  m_farClipDistance = distance;

  resetProjectionCache();
}

float Camera::getFarClipDistance() const
{
  return m_farClipDistance;
}

void Camera::setFOVy(float fov)
{
  m_FOVy = fov;

  resetProjectionCache();
}

float Camera::getFOVy() const
{
  return m_FOVy;
}

#include <boost/stacktrace.hpp>
#include <boost/exception/error_info.hpp>

glm::mat4x4 Camera::getProjectionMatrix()
{
  if (m_needProjectionMatrixCacheUpdate) {
    m_projectionMatrixCache = glm::perspective(m_FOVy, m_aspectRatio, m_nearClipDistance, m_farClipDistance);

    m_needFrustumUpdate = true;
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

    m_needFrustumUpdate = true;
  }

  return m_viewMatrixCache;
}

void Camera::resetProjectionCache()
{
  m_needProjectionMatrixCacheUpdate = true;
  m_needFrustumUpdate = true;
}
