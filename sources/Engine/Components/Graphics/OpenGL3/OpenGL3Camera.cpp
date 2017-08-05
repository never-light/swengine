#include "OpenGL3Camera.h"

OpenGL3Camera::OpenGL3Camera()
	: SceneObject(SceneObjectType::Camera),
	m_position(0.0f, 0.0f, 0.0f), 
	m_orientation()
{
	m_orientation = glm::normalize(m_orientation);
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

const vector3& OpenGL3Camera::getPosition() const {
	return m_position;
}

void OpenGL3Camera::setOrientation(const quaternion& orientation) {
	m_orientation = orientation;
}

const quaternion& OpenGL3Camera::getOrientation() const {
	return m_orientation;
}

void OpenGL3Camera::rotate(const quaternion& rotation, CoordinateSystemType relativeTo) {
	if (relativeTo == CoordinateSystemType::Local) {
		m_orientation = m_orientation * glm::normalize(rotation);
	}
	else if (relativeTo == CoordinateSystemType::World) {
		m_orientation = glm::normalize(rotation) * m_orientation;
	}
}

void OpenGL3Camera::rotate(const vector3& axis, float32 angle, CoordinateSystemType relativeTo) {
	rotate(glm::angleAxis(glm::radians(angle), axis), relativeTo);
}

void OpenGL3Camera::yaw(float32 angle) {
	rotate(vector3(0.0f, 1.0f, 0.0f), angle, CoordinateSystemType::World);
}

void OpenGL3Camera::pitch(float32 angle) {
	rotate(vector3(1.0f, 0.0f, 0.0f), angle, CoordinateSystemType::Local);
}

void OpenGL3Camera::roll(float32 angle) {
	rotate(vector3(0.0f, 0.0f, 1.0f), angle, CoordinateSystemType::Local);
}

vector3 OpenGL3Camera::getForwardDirection() const {
	return m_orientation * -vector3(0, 0, 1);
}

vector3 OpenGL3Camera::getRightDirection() const {
	return m_orientation * vector3(1, 0, 0);
}

vector3 OpenGL3Camera::getUpDirection() const {
	return m_orientation * vector3(0, 1, 0);
}

matrix4 OpenGL3Camera::getViewMatrix() const {
	vector3 position = m_position;
	quaternion orientation = m_orientation;

	if (m_parentSceneNode != nullptr) {
		position += m_parentSceneNode->getDerivedPosition();
		orientation *= glm::normalize(m_parentSceneNode->getDerivedOrientation());
	}

	glm::quat temp = glm::conjugate(orientation);

	matrix4 viewMatrix = matrix4();
	viewMatrix = glm::mat4_cast(temp);
	viewMatrix = glm::translate(viewMatrix, -position);

	return viewMatrix;
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

matrix4 OpenGL3Camera::getProjectionMatrix() const {
	return glm::perspective(m_FOVy, m_aspectRatio, m_nearClipDistance, m_farClipDistance);
}
