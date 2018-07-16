#pragma once

#include <variant>
#include <unordered_map>
#include <Engine\types.h>

#include "GpuProgram.h"
#include "Texture.h"

class Material {
public:
	Material();
	virtual ~Material();

	void setGpuProgram(GpuProgram* program);
	GpuProgram* getGpuProgram() const;

	virtual void create() = 0;
	virtual void destroy() = 0;

	virtual void bind() = 0;
	virtual void unbind() = 0;

	void setParameter(const std::string& name, int value);
	void setParameter(const std::string& name, const vector3& value);
	void setParameter(const std::string& name, const Texture* value);

	template<class T>
	T getParameter(const std::string& name) {
		return std::get<T>(m_parameters[name]);
	}
protected:
	GpuProgram* m_gpuProgram;

	using ParameterValue = std::variant<int, vector3, const Texture*>;
	std::unordered_map<std::string, ParameterValue> m_parameters;
};