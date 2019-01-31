#include "Light.h"

Light::Light(Type type)
	: 
	m_type(type), 
	m_position(),
	m_direction(),
	m_innerAngle(0.0f),
	m_outerAngle(0.0f),
	m_constant(1.0f),
	m_linear(0.9f),
	m_quadratic(0.1f),
	m_color(1.0f, 1.0f, 1.0f),
	m_ambientIntensity(0.0f),
	m_diffuseIntensity(1.0f)
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

void Light::setColor(const vector3& color) {
	m_color = color;
}

vector3 Light::getColor() const {
	return m_color;
}

void Light::setDiffuseIntensity(float intensity)
{
	m_diffuseIntensity = intensity;
}

float Light::getDiffuseIntensity() const
{
	return m_diffuseIntensity;
}

void Light::setAmbientIntensity(float intensity)
{
	m_ambientIntensity = intensity;
}

float Light::getAmbientIntensity() const
{
	return m_ambientIntensity;
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

void Light::setBoundingRadius(float radius)
{
	m_boundingRadius = radius;
}

float Light::getBoundingRadius() const
{
	return m_boundingRadius;
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