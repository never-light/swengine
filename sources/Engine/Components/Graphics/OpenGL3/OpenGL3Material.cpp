#include "OpenGL3Material.h"

OpenGL3Material::OpenGL3Material() {

}

OpenGL3Material::~OpenGL3Material() {

}


void OpenGL3Material::setAmbientColor(const Color& color) {
	m_ambientColor = color;
}


Color OpenGL3Material::getAmbientColor() const {
	return m_ambientColor;
}

void OpenGL3Material::setSpecularColor(const Color& color) {
	m_specularColor = color;
}

Color OpenGL3Material::getSpecularColor() const {
	return m_specularColor;
}

void OpenGL3Material::setDuffuseColor(const Color& color) {
	m_diffuseColor = color;
}

Color OpenGL3Material::getDuffuseColor() const {
	return m_diffuseColor;
}

void OpenGL3Material::setShininess(float32 shininess) {
	m_shininess = shininess;
}

float32 OpenGL3Material::getShininess() const {
	return m_shininess;
}