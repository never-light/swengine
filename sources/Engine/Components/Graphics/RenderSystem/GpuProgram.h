#pragma once

#include <string>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "Texture.h"
#include "Color.h"

struct GpuProgramParameter {
public:
	GpuProgramParameter() {

	}

	GpuProgramParameter(const std::string& type, const std::string& location, const std::string& name)
		: type(type), location(location), name(name)
	{
	
	}

	~GpuProgramParameter() {

	}

	const std::string& getType() const { return type; }
	const std::string& getLocation() const { return location; }
	const std::string& getName() const { return name; }

private:
	std::string type;
	std::string location;
	std::string name;
};

// { sectionName => { parameterName => parameterData, otherParameterName => parameterData } }
using GpuProgramParametersSection = std::unordered_map<std::string, GpuProgramParameter>;
using GpuProgramParametersList = std::unordered_map<std::string, GpuProgramParametersSection>;

class GpuProgram {
public:
	virtual void setParameter(const std::string& name, bool value) const = 0;
	virtual void setParameter(const std::string& name, int value) const = 0;
	virtual void setParameter(const std::string& name, float32 value) const = 0;
	virtual void setParameter(const std::string& name, const vector3& value) const = 0;
	virtual void setParameter(const std::string& name, const matrix4& value) const = 0;

	virtual const GpuProgramParametersList& getRequiredParameters() const = 0;
	virtual const GpuProgramParametersSection& getRequiredParametersSection(const std::string& name) const = 0;
	virtual bool hasRequiredParametersSection(const std::string& name) const = 0;
};