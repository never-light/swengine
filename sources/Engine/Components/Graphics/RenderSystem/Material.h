#pragma once

#include <variant>
#include <map>
#include <Engine\types.h>

#include "GpuProgram.h"
#include "Texture.h"

class Material {
public:
	Material();
	virtual ~Material();

	virtual void setGpuProgram(GpuProgram* program);
	virtual GpuProgram* getGpuProgram() const;

	virtual void setParameter(const std::string& name, GpuProgram::Parameter value);
	virtual GpuProgram::Parameter getParameter(const std::string& name) const;
	virtual bool hasParameter(const std::string& name) const;

	virtual const std::map<std::string, GpuProgram::Parameter>& getParameters() const;
private:
	GpuProgram* m_shader;

	std::map<std::string, GpuProgram::Parameter> m_parameters;
};