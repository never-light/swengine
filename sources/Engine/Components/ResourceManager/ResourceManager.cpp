#include "ResourceManager.h"

#include <regex>
#include <fstream>
#include <sstream>
#include <variant>

#include <Engine\Components\Debugging\Log.h>

#include <Engine\ServiceLocator.h>

#include "MaterialsDataParser.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {
	unloadResourcesSet<Material*>(m_materialsMap);
	unloadResourcesSet<Mesh*>(m_meshesMap);
	unloadResourcesSet<Sprite*>(m_spritesMap);
	unloadResourcesSet<GpuProgram*>(m_shadersMap);
	unloadResourcesSet<Texture*>(m_texturesMap);
}

Texture* ResourceManager::loadTexture(Texture::Type type, const std::string& filename) {
	if (type != Texture::Type::_2D)
		throw std::exception();

	if (m_texturesMap.find(filename) != m_texturesMap.end()) {
		return m_texturesMap.at(filename);
	}

	int width, height;
	int nrChannels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

	if (!data) {
		errlog() << "Texture loading error: " << filename;
		throw std::exception();
	}

	Texture* texture = ServiceLocator::getGraphicsManager()->createTexture(Texture::Type::_2D);

	texture->lock(true);
	texture->setPlainData(
		Texture::DataTarget::_2D,
		width, height,
		Texture::InternalFormat::RGBA,
		Texture::PixelFormat::RGBA,
		Texture::DataType::UnsignedByte,
		data
	);
	texture->setParameter(Texture::Parameter::MinFilter, Texture::ParameterValue::LinearMipmapLinear);
	texture->setParameter(Texture::Parameter::MagFilter, Texture::ParameterValue::Linear);
	texture->setParameter(Texture::Parameter::WrapS, Texture::ParameterValue::Repeat);
	texture->setParameter(Texture::Parameter::WrapT, Texture::ParameterValue::Repeat);

	texture->generateMipmaps();
	
	texture->unlock();

	stbi_image_free(data);

	m_texturesMap.insert(std::make_pair(filename, texture));

	return texture;
}

Texture* ResourceManager::loadTexture(Texture::Type type, const std::vector<std::string>& filenames) {
	if (type != Texture::Type::CubeMap)
		throw std::exception();

	if (filenames.size() != 6)
		throw std::exception();

	std::string cacheIndex = filenames[0] + filenames[1] + filenames[2] + filenames[3] + filenames[4] + filenames[5];

	if (m_texturesMap.find(cacheIndex) != m_texturesMap.end()) {
		return m_texturesMap.at(cacheIndex);
	}

	Texture* texture = ServiceLocator::getGraphicsManager()->createTexture(Texture::Type::CubeMap);

	texture->lock(true);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < filenames.size(); i++) {
		unsigned char *data = stbi_load(filenames[i].c_str(), &width, &height, &nrChannels, 0);

		if (!data) {
			errlog() << "Texture fragment loading error: " << filenames[i].c_str();
			stbi_image_free(data);
			throw std::exception();
		}

		texture->setPlainData(
			(Texture::DataTarget::CubeMapPositiveX + i),
			width, height,
			Texture::InternalFormat::RGBA,
			Texture::PixelFormat::RGB,
			Texture::DataType::UnsignedByte,
			data
		);

		stbi_image_free(data);
	}

	texture->setParameter(Texture::Parameter::MinFilter, Texture::ParameterValue::Linear);
	texture->setParameter(Texture::Parameter::MagFilter, Texture::ParameterValue::Linear);
	texture->setParameter(Texture::Parameter::WrapS, Texture::ParameterValue::ClampToEdge);
	texture->setParameter(Texture::Parameter::WrapT, Texture::ParameterValue::ClampToEdge);
	texture->setParameter(Texture::Parameter::WrapT, Texture::ParameterValue::ClampToEdge);

	texture->unlock();

	m_texturesMap.insert(std::make_pair(cacheIndex, texture));

	return texture;
}

GpuProgram* ResourceManager::loadShader(const std::string& filename) {
	if (m_shadersMap.find(filename) != m_shadersMap.end()) {
		return m_shadersMap.at(filename);
	}

	std::ifstream t(filename);
	std::string source((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	infolog() << "Loading shader " << filename << "...";
	GpuProgram* program = ServiceLocator::getGraphicsManager()->createGpuProgram(source);

	m_shadersMap.insert(std::make_pair(filename, program));

	return program;
}

Sprite* ResourceManager::loadSprite(const std::string& filename) {
	if (m_spritesMap.find(filename) != m_spritesMap.end()) {
		return m_spritesMap.at(filename);
	}

	Sprite* sprite = ServiceLocator::getGraphicsManager()->createSprite(loadTexture(Texture::Type::_2D, filename), loadShader("resources/shaders/sprite.sh"));
	sprite->setTexture(loadTexture(Texture::Type::_2D, filename));
	sprite->setShader(loadShader("resources/shaders/sprite.sh"));
	 
	m_spritesMap.insert(std::make_pair(filename, sprite));

	return sprite;
}

Mesh* ResourceManager::loadMesh(const std::string& filename) {
	if (m_meshesMap.find(filename) != m_meshesMap.end()) {
		return m_meshesMap.at(filename);
	}

	infolog() << "Loading mesh [" << filename << "]";
	std::vector<Mesh*> meshes;

	std::ifstream in(filename, std::ios::binary | std::ios::in);

	ModelFileHeaderData header;
	in.read((char*)&header, sizeof header);

	for (size_t i = 0; i < header.meshesCount; i++) {
		HardwareBuffer* geometryBuffer = ServiceLocator::getGraphicsManager()->createHardwareBuffer();
		geometryBuffer->lock();
		geometryBuffer->setVertexFormat(VertexFormat::P1N1UV);

		std::vector<VertexP1N1UV> vertices;

		ModelFileMeshData meshData;
		in.read((char*)&meshData, sizeof meshData);

		for (size_t j = 0; j < meshData.verticesCount; j++) {
			ModelFileVertexData vertexData;
			in.read((char*)&vertexData, sizeof vertexData);

			vertices.push_back(VertexP1N1UV(
				vector3(vertexData.x, vertexData.y, vertexData.z),
				vector3(vertexData.nx, vertexData.ny, vertexData.nz),
				vector2(vertexData.u, vertexData.v)
			));
		}

		geometryBuffer->setVerticesData(vertices.size(), sizeof(VertexP1N1UV), vertices.data());

		std::vector<uint32> indices;

		for (size_t j = 0; j < meshData.indicesCount; j++) {
			uint32 index;
			in.read((char*)&index, sizeof index);

			indices.push_back(index);
		}

		geometryBuffer->setIndicesData(indices.size(), sizeof(uint32), indices.data());
		geometryBuffer->unlock();

		Mesh* mesh = new Mesh(geometryBuffer);
		mesh->setName(meshData.name);

		meshes.push_back(mesh);
	}

	Mesh* mesh = new Mesh();
	for (Mesh* subMesh : meshes) {
		mesh->addSubMesh(subMesh);
	}

	m_meshesMap.insert(std::make_pair(filename, mesh));

	infolog() << "Mesh is loaded";

	return mesh;
}

void ResourceManager::loadMaterialsPackage(const std::string& filename) {
	infolog() << "Loading materials package [" << filename << "]";

	std::ifstream t(filename);
	std::string source((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	MaterialsDataParser parser(source);

	for (auto& materialData : parser.getMaterialsData()) {
		Material* material = ServiceLocator::getGraphicsManager()->createMaterial();

		for (auto& materialParameter : materialData.second) {
			if (materialParameter.first == "program") {
				material->setGpuProgram(loadShader(
					std::get<std::string>(materialParameter.second)
				));

				continue;
			}

			MaterialParameter paramValue;

			if (auto val = std::get_if<bool>(&materialParameter.second)) {
				paramValue = *val;
			}
			else if (auto val = std::get_if<int>(&materialParameter.second)) {
				paramValue = *val;
			}
			else if (auto val = std::get_if<real>(&materialParameter.second)) {
				paramValue = *val;
			}
			else if (auto val = std::get_if<std::string>(&materialParameter.second)) {
				continue;
			}
			else if (auto val = std::get_if<MaterialsDataParser::ParameterValueConstructor>(&materialParameter.second)) {
				MaterialsDataParser::ParameterValueConstructor constructor = *val;

				if (constructor.name == "Color") {
					if (constructor.args.size() != 3)
						throw std::exception();

					paramValue = Color(
						std::get<real>(constructor.args[0]),
						std::get<real>(constructor.args[1]),
						std::get<real>(constructor.args[2])
					);
				}
				else if (constructor.name == "Texture2D") {
					if (constructor.args.size() != 1)
						throw std::exception();

					paramValue = loadTexture(Texture::Type::_2D, std::get<std::string>(constructor.args[0]));
				}
				else if (constructor.name == "CubeMap") {
					if (constructor.args.size() != 6)
						throw std::exception();

					paramValue = loadTexture(Texture::Type::CubeMap, std::vector<std::string>{
						std::get<std::string>(constructor.args[0]),
						std::get<std::string>(constructor.args[1]),
						std::get<std::string>(constructor.args[2]),
						std::get<std::string>(constructor.args[3]),
						std::get<std::string>(constructor.args[4]),
						std::get<std::string>(constructor.args[5])
					});
				}
				else
					throw std::exception();
			}
			else
				throw std::exception();

			material->setParameter(materialParameter.first, paramValue);
		}

		m_materialsMap.insert({ materialData.first, material });
	}

	infolog() << "Materials package is loaded";
}

Material* ResourceManager::createMaterial(const std::string& name) {
	Material* material = ServiceLocator::getGraphicsManager()->createMaterial();
	m_materialsMap.insert(std::make_pair(name, material));

	return material;
}

Material* ResourceManager::getMaterial(const std::string& name) {
	return m_materialsMap.at(name);
}

template<class T>
void ResourceManager::unloadResourcesSet(std::unordered_map<std::string, T> map) {
	auto itr = map.begin();
	while (itr != map.end()) {
		delete itr->second;
		itr = map.erase(itr);
	}
}