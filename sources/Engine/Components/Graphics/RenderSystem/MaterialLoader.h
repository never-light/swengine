#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Material.h"

#include "GpuProgramLoader.h"
#include "TextureLoader.h"

struct MaterialParameterValue {
public:
	enum class Type {
		Number, String, Color, Texture, GpuProgram
	};

public:
	MaterialParameterValue() {}
	MaterialParameterValue(float number) : m_type(Type::String), m_number(number) {}
	MaterialParameterValue(std::string string) : m_type(Type::String), m_string(string) {}
	MaterialParameterValue(Color color) : m_type(Type::Color), m_color(color) {}
	MaterialParameterValue(Texture* texture) : m_type(Type::Texture), m_texture(texture) {}
	MaterialParameterValue(GpuProgram* program) : m_type(Type::GpuProgram), m_gpuProgram(program) {}

	float getNumber() const { return m_number; }
	std::string getString() const { return m_string; }
	Color getColor() const { return m_color; }
	Texture* getTexture() const { return m_texture; }
	GpuProgram* getGpuProgam() const { return m_gpuProgram; }
private:
	Type m_type;

	float m_number;
	std::string m_string;
	Color m_color;
	Texture* m_texture;
	GpuProgram* m_gpuProgram;
};

using MaterialParametersList = std::unordered_map<std::string, MaterialParameterValue>;

class MaterialLoader {
public:
	MaterialLoader();
	~MaterialLoader();

	virtual Material* createMaterial() = 0;
	virtual Material* createMaterial(const MaterialParametersList& parameters);

protected:
	virtual void setMaterialParameterIfExists(Material* material, const MaterialParametersList& parameters, const std::string& parameterName);
};