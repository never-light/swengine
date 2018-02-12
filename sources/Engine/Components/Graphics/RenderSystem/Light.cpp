#include "Light.h"

Light::Light(LightType type)
	: m_type(type), 
	m_needUpdateSpaceMatrix(true),
	m_position(),
	m_direction(),
	m_spaceMatrix(),
	m_innerAngle(0.0f),
	m_outerAngle(0.0f),
	m_constant(1.0f),
	m_linear(0.22f),
	m_quadratic(0.2f)
{

}

Light::~Light() {

}

void Light::setType(LightType type) {
	m_type = type;
}

LightType Light::getType() const {
	return m_type;
}

void Light::setAmbientColor(const Color& color) {
	m_ambientColor = color;
}

Color Light::getAmbientColor() const {
	return m_ambientColor;
}

void Light::setSpecularColor(const Color& color) {
	m_specularColor = color;
}

Color Light::getSpecularColor() const {
	return m_specularColor;
}

void Light::setDuffuseColor(const Color& color) {
	m_diffuseColor = color;
}

Color Light::getDuffuseColor() const {
	return m_diffuseColor;
}

void Light::setPosition(const vector3& position) {
	m_needUpdateSpaceMatrix = true;
	m_position = position;
}

vector3 Light::getPosition() const {
	return m_position;
}

void Light::setDirection(const vector3& direction) {
	m_needUpdateSpaceMatrix = true;
	m_direction = direction;
}

vector3 Light::getDirection() const {
	return m_direction;
}

void Light::setInnerAngle(real angle) {
	m_needUpdateSpaceMatrix = true;
	m_innerAngle = angle;
}

real Light::getInnerAngle() const {
	return m_innerAngle;
}

void Light::setOuterAngle(real angle) {
	m_needUpdateSpaceMatrix = true;
	m_outerAngle = angle;
}

real Light::getOuterAngle() const {
	return m_outerAngle;
}

void Light::setAttenuation(real constant, real linear, real quadratic) {
	m_constant = constant;
	m_linear = linear;
	m_quadratic = quadratic;
}

real Light::getAttenuationConstant() const {
	return m_constant;
}

real Light::getAttenuationLinear() const {
	return m_linear;
}

real Light::getAttenuationQuadratic() const {
	return m_quadratic;
}

const matrix4& Light::getSpaceMatrix() {
	if (m_needUpdateSpaceMatrix) {
		if (m_type == LightType::Directional) {
			matrix4 lightProjection, lightView;

			vector3 lightPos = -getDirection() * 150.0f;
			//lightPos = vector3(1.221149, 1.633645, -5.470305);

			float near_plane = 1.0f, far_plane = 177.5f;
			lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
			lightView = glm::lookAt(lightPos, lightPos + getDirection(), glm::vec3(0.0, 1.0, 0.0));
			m_spaceMatrix = lightProjection * lightView;
		}

		m_needUpdateSpaceMatrix = false;
	}

	return m_spaceMatrix;
}