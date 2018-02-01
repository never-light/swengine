#pragma once

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <Engine\types.h>

#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Engine\Components\Graphics\RenderSystem\Texture.h>
#include <Engine\Components\Graphics\RenderSystem\Material.h>
#include <Engine\Components\Graphics\RenderSystem\Sprite.h>
#include <Engine\Components\Graphics\RenderSystem\Mesh.h>

struct ModelFileHeaderData {
	std::uint32_t version;
	std::uint32_t meshesCount;
};

struct ModelFileMeshData {
	char name[64];
	std::uint32_t verticesCount;
	std::uint32_t indicesCount;
};

struct ModelFileVertexData {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	Sprite* loadSprite(const std::string&);
	Texture* loadTexture(const std::string&);
	GpuProgram* loadShader(const std::string&);
	Mesh* loadMesh(const std::string&);

	void loadMaterialsPackage(const std::string& filename);
	Material* createMaterial(const std::string& name);
	Material* getMaterial(const std::string& name);

private:
	template<class T>
	void unloadResourcesSet(std::unordered_map<std::string, T> map);

private:
	std::unordered_map<std::string, Texture*> m_texturesMap;
	std::unordered_map<std::string, GpuProgram*> m_shadersMap;
	std::unordered_map<std::string, Sprite*> m_spritesMap;
	std::unordered_map<std::string, Mesh*> m_meshesMap;

	std::unordered_map<std::string, Material*> m_materialsMap;
};