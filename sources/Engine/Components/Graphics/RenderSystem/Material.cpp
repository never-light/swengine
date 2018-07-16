#include "Material.h"

Material::Material() {

}

Material::~Material() {

}

void Material::setGpuProgram(GpuProgram* program) {
	m_gpuProgram = program;
}

GpuProgram* Material::getGpuProgram() const {
	return m_gpuProgram;
}

void Material::setParameter(const std::string & name, int value)
{
	m_parameters.insert({ name, value });
}

void Material::setParameter(const std::string & name, const vector3 & value)
{
	m_parameters.insert({ name, value });
}

void Material::setParameter(const std::string & name, const Texture * value)
{
	m_parameters.insert({ name, value });
}
