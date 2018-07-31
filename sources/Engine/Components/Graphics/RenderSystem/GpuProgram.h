#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "Texture.h"

class GpuProgram {
public:
	enum class ShaderType {
		Vertex, Fragment
	};
public:
	GpuProgram();
	virtual ~GpuProgram();

	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	virtual void addShader(ShaderType type, const std::string& source) = 0;
	virtual void link() = 0;

	virtual void setParameter(const std::string& name, bool value) = 0;
	virtual void setParameter(const std::string& name, int value) = 0;
	virtual void setParameter(const std::string& name, float value) = 0;
	virtual void setParameter(const std::string& name, const vector3& value) = 0;
	virtual void setParameter(const std::string& name, const vector4& value) = 0;
	virtual void setParameter(const std::string& name, const matrix4& value) = 0;
	virtual void setParameter(const std::string& name, Texture* value) = 0;
};