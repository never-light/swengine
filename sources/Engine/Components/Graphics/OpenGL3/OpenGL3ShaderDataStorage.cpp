#include "OpenGL3ShaderDataStorage.h"

OpenGL3ShaderDataStorage::OpenGL3ShaderDataStorage() {

}

OpenGL3ShaderDataStorage::~OpenGL3ShaderDataStorage() {

}

void OpenGL3ShaderDataStorage::setParameter(const std::string& name, const GpuProgram::Parameter& value) {
	m_data[name] = value;
}

const GpuProgram::Parameter& OpenGL3ShaderDataStorage::getParameter(const std::string& name) const {
	return m_data.at(name);
}

bool OpenGL3ShaderDataStorage::hasParameter(const std::string& name) const {
	return m_data.find(name) != m_data.end();
}

