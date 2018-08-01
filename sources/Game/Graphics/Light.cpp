#include "Light.h"

Light::Light(Type type)
	: m_type(type), 
	m_position(),
	m_direction(),
	m_innerAngle(0.0f),
	m_outerAngle(0.0f),
	m_constant(1.0f),
	m_linear(0.22f),
	m_quadratic(0.2f),
	m_boundingVolume()
{

}

Light::~Light() {

}

void Light::setType(Type type) {
	m_type = type;
}

Light::Type Light::getType() const {
	return m_type;
}

void Light::setSpecularColor(const vector3& color) {
	m_specularColor = color;
}

vector3 Light::getSpecularColor() const {
	return m_specularColor;
}

void Light::setDuffuseColor(const vector3& color) {
	m_diffuseColor = color;
}

vector3 Light::getDuffuseColor() const {
	return m_diffuseColor;
}

void Light::setPosition(const vector3& position) {
	m_position = position;
}

vector3 Light::getPosition() const {
	return m_position;
}

void Light::setDirection(const vector3& direction) {
	m_direction = direction;
}

vector3 Light::getDirection() const {
	return m_direction;
}

void Light::setInnerAngle(float angle) {
	m_innerAngle = angle;
}

float Light::getInnerAngle() const {
	return m_innerAngle;
}

void Light::setOuterAngle(float angle) {
	m_outerAngle = angle;
}

float Light::getOuterAngle() const {
	return m_outerAngle;
}

void Light::setBoundingVolume(const Sphere & volume)
{
	m_boundingVolume = volume;
}

Sphere Light::getBoundingVolume()
{
	return m_boundingVolume;
}

void Light::setAttenuation(float constant, float linear, float quadratic) {
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}

float Light::getAttenuationConstant() const {
	return m_constant;
}

float Light::getAttenuationLinear() const {
	return m_linear;
}

float Light::getAttenuationQuadratic() const {
	return m_quadratic;
}