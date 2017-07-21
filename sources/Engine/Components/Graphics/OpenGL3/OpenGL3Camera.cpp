#include "OpenGL3Camera.h"

OpenGL3Camera::OpenGL3Camera() 
	: m_position(0, 0, 0), m_orientation(0, 0, 0,  0) {

}

OpenGL3Camera::OpenGL3Camera(const vector3& position) : m_position(position) {

}

OpenGL3Camera::OpenGL3Camera(const vector3& position, const quaternion& orientation) 
	: m_position(position), m_orientation(orientation) 
{

}

OpenGL3Camera::~OpenGL3Camera() {

}

vector3 OpenGL3Camera::getPosition() const {
	return m_position;
}

quaternion OpenGL3Camera::getOrientation() const {
	return m_orientation;
}

void OpenGL3Camera::setPosition(const vector3& position) {
	m_position = position;
}

void OpenGL3Camera::setPosition(float32 x, float32 y, float32 z) {
	m_position = glm::vec3(x, y, z);
}

void OpenGL3Camera::setOrientation(const quaternion& orientation) {
	m_orientation = orientation;
}

void OpenGL3Camera::move(const vector3& movement) {
	m_position += movement;
}

void OpenGL3Camera::move(float32 x, float32 y, float32 z) {
	move(vector3(x, y, z));
}

void OpenGL3Camera::rotate(const quaternion& rotation) {
	m_orientation *= rotation;
}

void OpenGL3Camera::rotate(float32 angle, const vector3& axis) {
	rotate(glm::angleAxis(glm::radians(angle), glm::normalize(axis)));
}

void OpenGL3Camera::rotate(float32 angle, float32 x, float32 y, float32 z) {
	rotate(angle, vector3(x, y, z));
}

void OpenGL3Camera::yaw(float32 angle) {
	rotate(angle, vector3(0.0f, 1.0f, 0.0f));
}

void OpenGL3Camera::pitch(float32 angle) {
	rotate(angle, vector3(1.0f, 0.0f, 0.0f));
}

void OpenGL3Camera::roll(float32 angle) {
	rotate(angle, vector3(0.0f, 0.0f, 1.0f));
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
	matrix4 rotationMatrix = glm::mat4_cast(glm::normalize(m_orientation));
	matrix4 translationMatrix = glm::translate(glm::mat4(1.0), m_position);
	matrix4 scalingMatrix = glm::scale(glm::mat4(1.0), vector3(1,1,1));

	return rotationMatrix * translationMatrix * scalingMatrix;
}

matrix4 OpenGL3Camera::getProjectionMatrix() const {
	return glm::perspective(m_FOVy, m_aspectRatio, m_nearClipDistance, m_farClipDistance);
}