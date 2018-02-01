#include "OpenGL3Material.h"

OpenGL3Material::OpenGL3Material() {

}

OpenGL3Material::~OpenGL3Material() {

}

void OpenGL3Material::setGpuProgram(GpuProgram* program) {
	m_shader = static_cast<OpenGL3GpuProgram*>(program);
}

GpuProgram* OpenGL3Material::getGpuProgram() const {
	return m_shader;
}

void OpenGL3Material::setLightmap(LightmapType type, Texture* lightmap) {
	m_lightmaps[type] = static_cast<OpenGL3Texture*>(lightmap);;
}

Texture* OpenGL3Material::getLightmap(LightmapType type) const {
	return m_lightmaps.at(type);
}

bool OpenGL3Material::hasLightmap(LightmapType type) const {
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

void OpenGL3Material::setParameter(const std::string& name, MaterialParameter value) {
	m_parameters[name] = value;
}

MaterialParameter OpenGL3Material::getParameter(const std::string& name) const {
	return m_parameters.at(name);
}

bool OpenGL3Material::hasParameter(const std::string& name) const {
	return m_parameters.find(name) != m_parameters.end();
}