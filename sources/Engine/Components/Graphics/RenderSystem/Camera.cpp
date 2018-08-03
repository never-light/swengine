#include "Camera.h"

Camera::Camera() 
	: m_transform(new Transform())
{
	
}

Camera::~Camera() {

}

Transform* Camera::getTransform() const {
	return m_transform;
}

void Camera::setAspectRatio(float32 aspectRatio) {
	m_aspectRatio = aspectRatio;
}

float32 Camera::getAspectRatio() const {
	return m_aspectRatio;
}

void Camera::setNearClipDistance(float32 distance) {
	m_nearClipDistance = distance;
}

float32 Camera::getNearClipDistance() const {
	return m_nearClipDistance;
}

void Camera::setFarClipDistance(float32 distance) {
	m_farClipDistance = distance;
}

float32 Camera::getFarClipDistance() const {
	return m_farClipDistance;
}

void Camera::setFOVy(float32 FOVy) {
	m_FOVy = glm::radians(FOVy);
}

float32 Camera::getFOVy() const {
	return glm::degrees(m_FOVy);
}

matrix4 Camera::getProjectionMatrix() const {
	return glm::perspective(m_FOVy, m_aspectRatio, m_nearClipDistance, m_farClipDistance);
}

matrix4 Camera::getViewMatrix() const
{
	return glm::lookAt(m_transform->getPosition(),
		m_transform->getPosition() + m_transform->getFrontDirection(),
		m_transform->getUpDirection());
}