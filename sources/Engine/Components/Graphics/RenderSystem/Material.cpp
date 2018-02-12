#include "Material.h"

Material::Material() {

}

Material::~Material() {

}

void Material::setGpuProgram(GpuProgram* program) {
	m_shader = program;
}

GpuProgram* Material::getGpuProgram() const {
	return m_shader;
}

void Material::setParameter(const std::string& name, GpuProgram::Parameter value) {
	m_parameters[name] = value;
}

GpuProgram::Parameter Material::getParameter(const std::string& name) const {
	return m_parameters.at(name);
}

bool Material::hasParameter(const std::string& name) const {
	return m_parameters.find(name) != m_parameters.end();
}

const std::map<std::string, GpuProgram::Parameter>& Material::getParameters() const {
	return m_parameters;
}