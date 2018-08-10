#include "Transform.h"

Transform::Transform()
	: m_scale(1.0f, 1.0f, 1.0f),
	m_fixedYAxis(false),
	m_orientation(quaternion()),
	m_position(0.0f, 0.0f, 0.0f)
{

}

void Transform::move(float x, float y, float z)
{
	move(vector3(x, y, z));
}

void Transform::move(vector3 movement)
{
	m_position += movement;
}

void Transform::setPosition(float x, float y, float z)
{
	setPosition(vector3(x, y, z));
}

void Transform::setPosition(const vector3& position)
{
	m_position = position;
}

vector3 Transform::getPosition() const
{
	return m_position;
}

void Transform::scale(float x, float y, float z)
{
	scale(vector3(x, y, z));
}

void Transform::scale(const vector3& scale)
{
	m_scale *= scale;
}

void Transform::setScale(float x, float y, float z)
{
	setScale(vector3(x, y, z));
}

void Transform::setScale(const vector3& scale)
{
	m_scale = scale;
}

vector3 Transform::getScale() const
{
	return m_scale;
}

void Transform::rotate(float x, float y, float z, float angle)
{
	rotate(vector3(x, y, z), angle);
}

void Transform::rotate(const vector3& axis, float angle)
{
	m_orientation *= glm::angleAxis(glm::radians(angle), axis);
	m_orientation = glm::normalize(m_orientation);
}

void Transform::setOrientation(const quaternion& orientation)
{
	m_orientation = orientation;
}

quaternion Transform::getOrientation() const
{
	return m_orientation;
}

vector3 Transform::getFrontDirection() const {
	return glm::normalize(m_orientation * vector3(0.0f, 0.0f, -1.0f));
}

vector3 Transform::getRightDirection() const {
	return glm::normalize(m_orientation * vector3(1.0f, 0.0f, 0.0f));
}

vector3 Transform::getUpDirection() const {
	return glm::normalize(m_orientation * vector3(0.0f, 1.0f, 0.0f));
}

void Transform::fixYAxis(bool fixed)
{
	m_fixedYAxis = fixed;
}

bool Transform::isYAxisFixed() const
{
	return m_fixedYAxis;
}

void Transform::yaw(float angle)
{
	if (m_fixedYAxis) {
		m_orientation = glm::quat(vector3(0.0, glm::radians(angle), 0.0)) * m_orientation;
	}
	else {
		m_orientation *= glm::quat(vector3(0.0, glm::radians(angle), 0.0));
	}

	m_orientation = glm::normalize(m_orientation);
}

void Transform::pitch(float angle)
{
	rotate(vector3(1.0f, 0.0f, 0.0f), angle);
}

void Transform::roll(float angle)
{
	rotate(vector3(0.0f, 0.0f, 1.0f), angle);
}

float Transform::getYawValue() const {
	return glm::degrees(glm::eulerAngles(m_orientation).y);
}

float Transform::getPitchValue() const {
	return glm::degrees(glm::eulerAngles(m_orientation).x);
}

float Transform::getRollValue() const {
	return glm::degrees(glm::eulerAngles(m_orientation).z);
}

void Transform::lookAt(float x, float y, float z)
{
	lookAt(vector3(x, y, z));
}

void Transform::lookAt(const vector3& target)
{
	matrix3 m;
	m[2] = glm::normalize(m_position - target);
	m[0] = glm::normalize(glm::cross(vector3(0.0f, 1.0f, 0.0f), m[2]));
	m[1] = glm::cross(m[2], m[0]);

	m_orientation = quat_cast(m);
}

matrix4 Transform::getTransformationMatrix() const
{
	return glm::translate(matrix4(), m_position) * glm::toMat4(m_orientation) * glm::scale(matrix4(), m_scale);
}
