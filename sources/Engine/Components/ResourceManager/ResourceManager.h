#pragma once

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Graphics\GraphicsResourceFactory.h>

#include <Engine\Components\Graphics\RenderSystem\GpuProgram.h>
#include <Engine\Components\Graphics\RenderSystem\Texture.h>
#include <Engine\Components\Graphics\RenderSystem\Material.h>
#include <Engine\Components\Graphics\RenderSystem\Mesh.h>

#include <variant>
#include <string>
#include <unordered_map>
#include <memory>

class ResourceManager {
public:
	ResourceManager(GraphicsResourceFactory* graphicsResourceFactory);
	~ResourceManager();

	Texture* load2DTexture(const std::string& name, const std::string& filename, Texture::InternalFormat internalFormat = Texture::InternalFormat::RGBA);
	Texture* loadCubeMapTexture(const std::string& name, const std::vector<std::string>& filenames);

	GpuProgram* loadGpuProgram(const std::string& name, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	Mesh* loadMesh(const std::string& name, const std::string& filename);

	Material* createMaterial(const std::string& name);

	bool isResourceLoaded(const std::string& name) const;

	template<class T> 
	T* getResource(const std::string& name) {
		return std::get<std::unique_ptr<T>>(m_resourcesStore.at(name)).get();
	}

private:
	using Resource = std::variant<std::unique_ptr<Texture>, std::unique_ptr<GpuProgram>, std::unique_ptr<Mesh>, std::unique_ptr<Material>>;
	std::unordered_map<std::string, Resource> m_resourcesStore;
private:
	GraphicsResourceFactory* m_graphicsResourceFactory;
};