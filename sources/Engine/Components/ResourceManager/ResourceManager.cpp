#include "ResourceManager.h"

#include <regex>
#include <fstream>
#include <sstream>
#include <variant>
#include <bitset>
#include <array>

#include <Engine\Components\Debugging\Log.h>

#include "MeshLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ResourceManager::ResourceManager(GraphicsResourceFactory* graphicsResourceFactory) 
	: m_graphicsResourceFactory(graphicsResourceFactory) 
{
	
}

ResourceManager::~ResourceManager() {

}

Texture* ResourceManager::load2DTexture(const std::string& name, const std::string& filename, Texture::InternalFormat internalFormat) {
	if (isResourceLoaded(name))
		throw std::exception(("Resource " + name + " already loaded").c_str());

	int width, height;
	int nrChannels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

	if (!data) {
		throw std::exception(("Texture loading error, file is not available [" + name + "]").c_str());
	}

	Texture::PixelFormat pixelFormat = Texture::PixelFormat::RGBA;

	Texture* texture = m_graphicsResourceFactory->createTexture();
	texture->setTarget(Texture::Target::_2D);
	texture->setInternalFormat(internalFormat);
	texture->setSize(width, height);

	texture->create();
	texture->bind();

	texture->setData(pixelFormat, Texture::PixelDataType::UnsignedByte, (const std::byte*)data);

	texture->generateMipMaps();
	texture->setMinificationFilter(Texture::Filter::LinearMipmapLinear);
	texture->setMagnificationFilter(Texture::Filter::Linear);

	texture->setWrapMode(Texture::WrapMode::Repeat);
	
	texture->unbind();

	stbi_image_free(data);

	m_resourcesStore.insert({ name, std::unique_ptr<Texture>(texture) });

	return texture;
}

Texture* ResourceManager::loadCubeMapTexture(const std::string& name, const std::vector<std::string>& filenames) {
	if (isResourceLoaded(name))
		throw std::exception(("Resource " + name + " already loaded").c_str());

	Texture* texture = m_graphicsResourceFactory->createTexture();
	texture->setTarget(Texture::Target::CubeMap);

	int width, height, nrChannels;

	for (unsigned int i = 0; i < filenames.size(); i++) {
		infolog() << "Loading cubemap part " << filenames[i] << "...";
		unsigned char *data = stbi_load(filenames[i].c_str(), &width, &height, &nrChannels, 0);

		if (!data) {
			errlog() << "Cubemap part loading error: " << filenames[i].c_str();
			stbi_image_free(data);
			throw std::exception();
		}

		if (i == 0) {
			texture->setSize(width, height);
			texture->setInternalFormat(Texture::InternalFormat::SRGB);
			texture->create();
			texture->bind();
		}

		Texture::CubeMapFace face = (Texture::CubeMapFace)((int)Texture::CubeMapFace::PositiveX + i);
		texture->setData(face, Texture::PixelFormat::RGB, Texture::PixelDataType::UnsignedByte, (const std::byte*)data);

		stbi_image_free(data);
		infolog() << "Cubemap part is loaded";
	}

	texture->setMinificationFilter(Texture::Filter::Linear);
	texture->setMagnificationFilter(Texture::Filter::Linear);

	texture->setWrapMode(Texture::WrapMode::ClampToEdge);
	texture->unbind();

	m_resourcesStore.insert({ name, std::unique_ptr<Texture>(texture) });

	return texture;
}

GpuProgram* ResourceManager::loadGpuProgram(const std::string& name, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename) {
	if (isResourceLoaded(name))
		throw std::exception(("Resource " + name + " already loaded").c_str());

	std::ifstream vertexShaderStream(vertexShaderFilename);

	if (!vertexShaderStream.is_open())
		throw std::exception(("GpuProgram loading error, vertex shader file is not available [" + name + "]").c_str());

	std::string vertexShaderSource((std::istreambuf_iterator<char>(vertexShaderStream)),
		std::istreambuf_iterator<char>());

	std::ifstream fragmentShaderStream(fragmentShaderFilename);

	if (!fragmentShaderStream.is_open())
		throw std::exception(("GpuProgram loading error, fragment shader file is not available [" + name + "]").c_str());

	std::string fragmentShaderSource((std::istreambuf_iterator<char>(fragmentShaderStream)),
		std::istreambuf_iterator<char>());


	GpuProgram* gpuProgram = m_graphicsResourceFactory->createGpuProgram();
	gpuProgram->create();
	gpuProgram->addShader(GpuProgram::ShaderType::Vertex, vertexShaderSource);
	gpuProgram->addShader(GpuProgram::ShaderType::Fragment, fragmentShaderSource);

	gpuProgram->link();

	m_resourcesStore.insert({ name, std::unique_ptr<GpuProgram>(gpuProgram) });

	return gpuProgram;
}

Mesh* ResourceManager::loadMesh(const std::string& name, const std::string& filename) {
	if (isResourceLoaded(name))
		throw std::exception(("Resource " + name + " already loaded").c_str());

	try {
		Mesh* mesh = m_graphicsResourceFactory->createMesh();
		MeshLoader::load(filename, mesh);

		m_resourcesStore.insert({ name, std::unique_ptr<Mesh>(mesh) });

		return mesh;
	}
	catch (const std::exception& e) {
		throw std::exception(("Mesh loading error, file is not available [" + name + "]").c_str());
	}
}

Material* ResourceManager::createMaterial(const std::string& name) {
	if (isResourceLoaded(name))
		throw std::exception(("Resource " + name + " already loaded").c_str());

	Material* material = m_graphicsResourceFactory->createMaterial();
	material->create();

	m_resourcesStore.insert({ name, std::unique_ptr<Material>(material) });

	return material;
}

bool ResourceManager::isResourceLoaded(const std::string& name) const
{
	return m_resourcesStore.find(name) != m_resourcesStore.end();
}
