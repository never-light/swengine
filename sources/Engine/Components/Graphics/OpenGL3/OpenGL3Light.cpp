#include "OpenGL3Light.h"

OpenGL3Light::OpenGL3Light() {

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