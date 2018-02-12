#pragma once

#include <string>
#include <variant>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>

#include "OpenGL3.h"
#include "OpenGL3GpuProgram.h"

class OpenGL3ShaderDataStorage {
public:
	OpenGL3ShaderDataStorage();
	~OpenGL3ShaderDataStorage();
	
	void setParameter(const std::string& name, const GpuProgram::Parameter& value);
	const GpuProgram::Parameter& getParameter(const std::string& name) const;
	bool hasParameter(const std::string& name) const;
private:
	std::unordered_map<std::string, GpuProgram::Parameter> m_data;
};