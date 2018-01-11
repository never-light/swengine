#include "Camera.h"

Camera::Camera() : 
	m_position(0.0f, 0.0f, 0.0f),
	m_orientation(vector3(0.0f, 0.0f, 0.0f))
{
	
}

Camera::~Camera() {

}

void Camera::setPosition(const vector3& position) {
	m_position = position;
}

void Camera::setPosition(float32 x, float32 y, float32 z) {
	m_position = glm::vec3(x, y, z);
}

void Camera::move(const vector3& movement) {
	m_position += movement;
}

void Camera::move(float32 x, float32 y, float32 z) {
	move(vector3(x, y, z));
}

const vector3& Camera::getPosition() const {
	return m_position;
}

quaternion Camera::getOrientation() const {
	return m_orientation;
}

void Camera::setOrientation(const quaternion& orientation) {
	m_orientation = orientation;
}

void Camera::setYawValue(real value) {
	vector3 angles = glm::eulerAngles(m_orientation);
	angles.y = glm::radians(value);

	m_orientation = quaternion(angles);
}

void Camera::setPitchValue(real value) {
	vector3 angles = glm::eulerAngles(m_orientation);
	angles.x = glm::radians(value);

	m_orientation = quaternion(angles);
}

void Camera::setRollValue(real value) {
	vector3 angles = glm::eulerAngles(m_orientation);
	angles.z = glm::radians(value);

	m_orientation = quaternion(angles);
}

void Camera::yaw(real angle) {
	m_orientation = glm::quat(vector3(0.0, glm::radians(angle), 0.0)) * m_orientation;
}

void Camera::pitch(real angle) {
	m_orientation *= glm::quat(vector3(glm::radians(angle), 0.0, 0.0));
}

void Camera::roll(real angle) {
	m_orientation *= glm::quat(vector3(0.0, 0.0, glm::radians(angle)));
}

real Camera::getYawValue() const {
	return glm::degrees(glm::eulerAngles(m_orientation).y);
}

real Camera::getPitchValue() const {
	return glm::degrees(glm::eulerAngles(m_orientation).x);
}

real Camera::getRollValue() const {
	return glm::degrees(glm::eulerAngles(m_orientation).z);
}

void Camera::lookAt(const vector3& target) {
	matrix3 m;
	m[2] = glm::normalize(m_position - target);
	m[0] = glm::normalize(glm::cross(vector3(0.0f, 1.0f, 0.0f), m[2]));
	m[1] = glm::cross(m[2], m[0]);

	m_orientation = quat_cast(m);
}

void Camera::lookAt(real x, real y, real z) {
	lookAt(vector3(x, y, z));
}

vector3 Camera::getFrontDirection() const {
	return m_orientation * vector3(0.0f, 0.0f, -1.0f);
}

vector3 Camera::getRightDirection() const {
	return m_orientation * vector3(1.0f, 0.0f, 0.0f);
}

vector3 Camera::getUpDirection() const {
	return m_orientation * vector3(0.0f, 1.0f, 0.0f);
}

matrix4 Camera::getViewMatrix() const {
	return glm::lookAt(m_position, m_position + getFrontDirection(), getUpDirection());
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
	m_FOVy = FOVy;
}

float32 Camera::getFOVy() const {
	return m_FOVy;
}

matrix4 Camera::getProjectionMatrix() const {
	return glm::perspective(m_FOVy, m_aspectRatio, m_nearClipDistance, m_farClipDistance);
}
