#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <Engine\types.h>

#include <Engine\Components\Graphics\RenderSystem\TextureLoader.h>
#include <Engine\Components\Graphics\RenderSystem\GpuProgramLoader.h>
#include <Engine\Components\Graphics\RenderSystem\MaterialLoader.h>
#include <Engine\Components\Graphics\RenderSystem\MeshLoader.h>
#include <Engine\Components\Graphics\RenderSystem\SpriteLoader.h>

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
	TextureLoader* m_textureLoader;
	GpuProgramLoader* m_gpuProgramLoader;
	MaterialLoader* m_materialLoader;
	MeshLoader* m_meshLoader;
	SpriteLoader* m_spriteLoader;

private:
	std::unordered_map<std::string, Texture*> m_texturesMap;
	std::unordered_map<std::string, GpuProgram*> m_shadersMap;
	std::unordered_map<std::string, Sprite*> m_spritesMap;
	std::unordered_map<std::string, Mesh*> m_meshesMap;

	std::unordered_map<std::string, Material*> m_materialsMap;
};