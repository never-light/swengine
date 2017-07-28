#include "OpenGL3Spotlight.h"

OpenGL3Spotlight::OpenGL3Spotlight() : OpenGL3Light() {
	setType(LightType::Spotlight);
	setAttenuation(1.0f, 0.09f, 0.032f);
}

OpenGL3Spotlight::~OpenGL3Spotlight() {

}

void OpenGL3Spotlight::setPosition(const vector3& position) {
	m_position = position;
}

vector3 OpenGL3Spotlight::getPosition() const {
	return m_position;
}

void OpenGL3Spotlight::setDirection(const vector3& direction) {
	m_direction = direction;
}

vector3 OpenGL3Spotlight::getDirection() const {
	return m_direction;
}

void OpenGL3Spotlight::setInnerAngle(float32 angle) {
	m_innerAngle = angle;
}

float32 OpenGL3Spotlight::getInnerAngle() const {
	return m_innerAngle;
}

void OpenGL3Spotlight::setOuterAngle(float32 angle) {
	m_outerAngle = angle;
}

float32 OpenGL3Spotlight::getOuterAngle() const {
	return m_outerAngle;
}

void OpenGL3Spotlight::setAttenuation(float32 constant, float32 linear, float32 quadratic) {
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}

float32 OpenGL3Spotlight::getAttenuationConstant() const {
	return m_constant;
}

float32 OpenGL3Spotlight::getAttenuationLinear() const {
	return m_linear;
}

float32 OpenGL3Spotlight::getAttenuationQuadratic() const {
	return m_quadratic;
}