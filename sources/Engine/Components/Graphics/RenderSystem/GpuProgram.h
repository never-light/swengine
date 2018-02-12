#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "Texture.h"
#include "Color.h"

struct GpuProgramRequiredParameter {
public:
	GpuProgramRequiredParameter() {

	}

	GpuProgramRequiredParameter(const std::string& type, const std::string& location, const std::string& name)
		: type(type), location(location), name(name)
	{
	
	}

	~GpuProgramRequiredParameter() {

	}

	const std::string& getType() const { return type; }
	const std::string& getLocation() const { return location; }
	const std::string& getName() const { return name; }

private:
	std::string type;
	std::string location;
	std::string name;
};

class GpuProgram {
public:
	using Parameter = std::variant<bool, int, real, vector3, matrix4, Texture*>;
public:
	GpuProgram(const std::unordered_map<std::string, std::string>& sources);
	virtual ~GpuProgram();

	virtual void setParameter(const std::string& name, bool value) = 0;
	virtual void setParameter(const std::string& name, int value) = 0;
	virtual void setParameter(const std::string& name, float32 value) = 0;
	virtual void setParameter(const std::string& name, const vector3& value) = 0;
	virtual void setParameter(const std::string& name, const matrix4& value) = 0;
	virtual void setParameter(const std::string& name, Texture* value) = 0;

	virtual const std::vector<GpuProgramRequiredParameter>& getRequiredParameters() const = 0;
	virtual void setRequiredParameters(const std::vector<GpuProgramRequiredParameter>& parameters) = 0;
};