#include "OpenGL3Light.h"

OpenGL3Light::OpenGL3Light(LightType type) 
	: m_type(type) {

}

OpenGL3Light::~OpenGL3Light() {

}

void OpenGL3Light::setType(LightType type) {
	m_type = type;
}

LightType OpenGL3Light::getType() const {
	return m_type;
}

void OpenGL3Light::setAmbientColor(const Color& color) {
	m_ambientColor = color;
}

Color OpenGL3Light::getAmbientColor() const {
	return m_ambientColor;
}

void OpenGL3Light::setSpecularColor(const Color& color) {
	m_specularColor = color;
}

Color OpenGL3Light::getSpecularColor() const {
	return m_specularColor;
}

void OpenGL3Light::setDuffuseColor(const Color& color) {
	m_diffuseColor = color;
}

Color OpenGL3Light::getDuffuseColor() const {
	return m_diffuseColor;
}

void OpenGL3Light::setPosition(const vector3& position) {
	m_position = position;
}

vector3 OpenGL3Light::getPosition() const {
	return m_position;
}

void OpenGL3Light::setDirection(const vector3& direction) {
	m_direction = direction;
}

vector3 OpenGL3Light::getDirection() const {
	return m_direction;
}

void OpenGL3Light::setInnerAngle(float32 angle) {
	m_innerAngle = angle;
}

float32 OpenGL3Light::getInnerAngle() const {
	return m_innerAngle;
}

void OpenGL3Light::setOuterAngle(float32 angle) {
	m_outerAngle = angle;
}

float32 OpenGL3Light::getOuterAngle() const {
	return m_outerAngle;
}

void OpenGL3Light::setAttenuation(float32 constant, float32 linear, float32 quadratic) {
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}

float32 OpenGL3Light::getAttenuationConstant() const {
	return m_constant;
}

float32 OpenGL3Light::getAttenuationLinear() const {
	return m_linear;
}

float32 OpenGL3Light::getAttenuationQuadratic() const {
	return m_quadratic;
}