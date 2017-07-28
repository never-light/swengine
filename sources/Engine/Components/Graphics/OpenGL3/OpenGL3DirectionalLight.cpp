#include "OpenGL3DirectionalLight.h"

OpenGL3DirectionalLight::OpenGL3DirectionalLight() : OpenGL3Light() {
	setType(LightType::Directional);
}

OpenGL3DirectionalLight::~OpenGL3DirectionalLight() {

}

void OpenGL3DirectionalLight::setDirection(const vector3& direction) {
	m_direction = direction;
}

vector3 OpenGL3DirectionalLight::getDirection() const {
	return m_direction;
}