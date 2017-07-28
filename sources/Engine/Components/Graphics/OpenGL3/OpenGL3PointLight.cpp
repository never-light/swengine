#include "OpenGL3PointLight.h"

OpenGL3PointLight::OpenGL3PointLight() : OpenGL3Light() {
	setType(LightType::Point);
	setAttenuation(1.0f, 0.14f, 0.07f);
}

OpenGL3PointLight::~OpenGL3PointLight() {

}

void OpenGL3PointLight::setPosition(const vector3& position) {
	m_position = position;
}

vector3 OpenGL3PointLight::getPosition() const {
	return m_position;
}

void OpenGL3PointLight::setAttenuation(float32 constant, float32 linear, float32 quadratic) {
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}

float32 OpenGL3PointLight::getAttenuationConstant() const {
	return m_constant;
}

float32 OpenGL3PointLight::getAttenuationLinear() const {
	return m_linear;
}

float32 OpenGL3PointLight::getAttenuationQuadratic() const {
	return m_quadratic;
}