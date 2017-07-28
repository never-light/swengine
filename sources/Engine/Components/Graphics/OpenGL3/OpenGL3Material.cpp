#include "OpenGL3Material.h"

OpenGL3Material::OpenGL3Material() {

}

OpenGL3Material::~OpenGL3Material() {

}

void OpenGL3Material::setShader(OpenGL3Shader* shader) {
	m_shader = shader;
}

OpenGL3Shader* OpenGL3Material::getShader() const {
	return m_shader;
}

void OpenGL3Material::setLightmap(LightmapType type, OpenGL3Texture* lightmap) {
	m_lightmaps[type] = lightmap;
}

OpenGL3Texture* OpenGL3Material::getLightmap(LightmapType type) const {
	return m_lightmaps.at(type);
}

bool OpenGL3Material::isLightmapExists(LightmapType type) const {
	return m_lightmaps.find(type) != m_lightmaps.end();
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

void OpenGL3Material::setAmbientColor(const Color& color) {
	m_ambientColor = color;
}

Color OpenGL3Material::getAmbientColor() const {
	return m_ambientColor;
}

void OpenGL3Material::setShininess(float32 shininess) {
	m_shininess = shininess;
}

float32 OpenGL3Material::getShininess() const {
	return m_shininess;
}