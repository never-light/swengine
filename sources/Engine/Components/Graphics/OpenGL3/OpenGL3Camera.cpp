#include "OpenGL3Camera.h"

OpenGL3Camera::OpenGL3Camera() 
	: m_position(0.0f, 0.0f, 0.0f), m_yaw(0.0f), m_pitch(0.0f), m_maxPitchValue(90.0f) {

}

OpenGL3Camera::~OpenGL3Camera() {

}

void OpenGL3Camera::setPosition(const vector3& position) {
	m_position = position;
}

void OpenGL3Camera::setPosition(float32 x, float32 y, float32 z) {
	m_position = glm::vec3(x, y, z);
}

void OpenGL3Camera::move(const vector3& movement) {
	m_position += movement;
}

void OpenGL3Camera::move(float32 x, float32 y, float32 z) {
	move(vector3(x, y, z));
}

vector3 OpenGL3Camera::getPosition() const {
	return m_position;
}

void OpenGL3Camera::lookAt(const vector3& target) {
	glm::vec3 direction = glm::normalize(target - m_position);
	m_yaw = glm::radians(asinf(-direction.y));
	m_pitch = -glm::radians(atan2f(-direction.x, -direction.z));
	normalizeAngles();
}

void OpenGL3Camera::setMaxPitchValue(float32 pitchValue) {
	m_maxPitchValue = pitchValue;
}

float32 OpenGL3Camera::getMaxPitchValue() const {
	return m_maxPitchValue;
}

void OpenGL3Camera::yaw(float32 angle) {
	m_yaw += angle;
	normalizeAngles();
}

void OpenGL3Camera::pitch(float32 angle) {
	m_pitch += angle;
	normalizeAngles();
}

void OpenGL3Camera::normalizeAngles() {
	m_yaw = fmodf(m_yaw, 360.0f);

	if (m_yaw < 0.0f) {
		m_yaw += 360.0f;
	}

	if (m_pitch > m_maxPitchValue) {
		m_pitch = m_maxPitchValue;
	}
	else if (m_pitch < -m_maxPitchValue) {
		m_pitch = -m_maxPitchValue;
	}
}

matrix4 OpenGL3Camera::getOrientationMatrix() const {
	matrix4 orientation;
	orientation = glm::rotate(orientation, glm::radians(m_pitch), glm::vec3(1, 0, 0));
	orientation = glm::rotate(orientation, glm::radians(m_yaw), glm::vec3(0, 1, 0));
	return orientation;
}

vector3 OpenGL3Camera::getForwardDirection() const {
	vector3 forward = glm::inverse(getOrientationMatrix()) * vector4(0, 0, -1, 1);
	return vector3(forward);
}

vector3 OpenGL3Camera::getRightDirection() const {
	vector4 right = glm::inverse(getOrientationMatrix()) * vector4(1, 0, 0, 1);
	return vector3(right);
}

vector3 OpenGL3Camera::getUpDirection() const {
	vector4 up = glm::inverse(getOrientationMatrix()) * vector4(0, 1, 0, 1);
	return vector3(up);
}

void OpenGL3Camera::setAspectRatio(float32 aspectRatio) {
	m_aspectRatio = aspectRatio;
}

float32 OpenGL3Camera::getAspectRatio() const {
	return m_aspectRatio;
}

void OpenGL3Camera::setNearClipDistance(float32 distance) {
	m_nearClipDistance = distance;
}

float32 OpenGL3Camera::getNearClipDistance() const {
	return m_nearClipDistance;
}

void OpenGL3Camera::setFarClipDistance(float32 distance) {
	m_farClipDistance = distance;
}

float32 OpenGL3Camera::getFarClipDistance() const {
	return m_farClipDistance;
}

void OpenGL3Camera::setFOVy(float32 FOVy) {
	m_FOVy = FOVy;
}

float32 OpenGL3Camera::getFOVy() const {
	return m_FOVy;
}

matrix4 OpenGL3Camera::getViewMatrix() const {
	return getOrientationMatrix() * glm::translate(matrix4(), -m_position);
}

matrix4 OpenGL3Camera::getProjectionMatrix() const {
	return glm::perspective(m_FOVy, m_aspectRatio, m_nearClipDistance, m_farClipDistance);
}