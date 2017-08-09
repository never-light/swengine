#pragma once

#include <vector>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Graphics\Texture.h>
#include <Engine\Components\Graphics\Material.h>

class ResourceManager;

struct MaterialParameterValue {
public:
	enum class Type {
		Number, String, Color, Texture
	};

public:
	MaterialParameterValue(float number) : m_type(Type::String), m_number(number) {}
	MaterialParameterValue(std::string string) : m_type(Type::String), m_string(string) {}
	MaterialParameterValue(Color color) : m_type(Type::Color), m_color(color) {}
	MaterialParameterValue(Texture* texture) : m_type(Type::Texture), m_texture(texture) {}

	float getNumber() const { return m_number; };
	std::string getString() const { return m_string; };
	Color getColor() const { return m_color; }
	Texture* getTexture() const { return m_texture; };
private:
	Type m_type;

	float m_number;
	std::string m_string;
	Color m_color;
	Texture* m_texture;
};

class MaterialLoader {
public:
	MaterialLoader();
	~MaterialLoader();

	std::unordered_map<std::string, Material*> loadPackage(ResourceManager* resourceManager, const std::string& path);

private:
	Material* createMaterial(const std::string& name, const std::unordered_map<std::string, MaterialParameterValue>& parameters);
	void setMaterialParameterIfExists(Material* material, const std::unordered_map<std::string, MaterialParameterValue>& parameters, const std::string& parameterName);

private:
	ResourceManager* m_resourceManager;
};