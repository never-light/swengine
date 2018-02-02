#include "ResourceManager.h"

#include <regex>
#include <fstream>
#include <sstream>
#include <variant>

#include <Engine\Components\Debugging\Log.h>

#include <Engine\ServiceLocator.h>

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

Texture* ResourceManager::loadTexture(const std::string& filename) {
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

	Texture* texture = ServiceLocator::getGraphicsManager()->createTexture(width, height, data);
	m_texturesMap.insert(std::make_pair(filename, texture));

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

	Sprite* sprite = ServiceLocator::getGraphicsManager()->createSprite(loadTexture(filename), loadShader("resources/shaders/sprite.sh"));
	sprite->setTexture(loadTexture(filename));
	sprite->setShader(loadShader("resources/shaders/sprite.sh"));
	 
	m_spritesMap.insert(std::make_pair(filename, sprite));

	return sprite;
}

Mesh* ResourceManager::loadMesh(const std::string& filename) {
	if (m_meshesMap.find(filename) != m_meshesMap.end()) {
		return m_meshesMap.at(filename);
	}

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

	return mesh;
}

void ResourceManager::loadMaterialsPackage(const std::string& filename) {
	std::unordered_map<std::string, Material*> package;

	std::ifstream in(filename);
	std::string currentLine;

	std::regex materialNameRegex("^\\[([a-zA-Z0-9]+)\\]$");
	std::regex materialParameterNumberRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+([-+]?[0-9]*\\.?[0-9]+)$");
	std::regex materialParameterBoolRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+(true|false)$");
	std::regex materialParameterStringRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+\\\"([0-9a-zA-Z\\.\\\\\\/_]+)\\\"$");
	std::regex materialParameterColorRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+color\\(([-+]?[0-9]*\\.?[0-9]+),[\\s]+([-+]?[0-9]*\\.?[0-9]+),[\\s]+([-+]?[0-9]*\\.?[0-9]+)\\)$");
	std::regex materialParameterTextureRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+texture\\(\\\"([0-9a-zA-Z\\.\\\\\\/_]+)\\\"\\)$");

	std::smatch match;

	std::string currentMaterialName;
	std::unordered_map<std::string, std::string> materialsData;

	while (std::getline(in, currentLine)) {
		if (currentLine.empty()) {
			continue;
		}

		if (std::regex_search(currentLine, match, materialNameRegex)) {
			currentMaterialName = match[1];
			materialsData.insert({ currentMaterialName, "" });
			continue;
		}

		materialsData[currentMaterialName] += (currentLine + "\n");
	}

	for (auto& materialData : materialsData) {
		Material* material = ServiceLocator::getGraphicsManager()->createMaterial();

		std::stringstream parametersStream(materialData.second);
		std::string part;

		while (std::getline(parametersStream, part, '\n')) {
			MaterialParameter v;

			// String value
			if (std::regex_search(part, match, materialParameterStringRegex)) {
				if (match[1] == "program") {
					material->setGpuProgram(loadShader(std::string(match[2])));
					continue;
				}
			}
			// Number Value
			else if (std::regex_search(part, match, materialParameterNumberRegex)) {
				v = (real)std::atof(std::string(match[2]).c_str());
			}
			// Bool Value
			else if (std::regex_search(part, match, materialParameterBoolRegex)) {
				v = (match[2] == "true");
			}
			// Color value
			else if (std::regex_search(part, match, materialParameterColorRegex)) {
				v = Color(
					std::atof(std::string(match[2]).c_str()), // r
					std::atof(std::string(match[3]).c_str()), // g
					std::atof(std::string(match[4]).c_str()) // b
				);
			}
			// Texture value
			else if (std::regex_search(part, match, materialParameterTextureRegex)) {
				v = loadTexture(match[2]);
			}
			else {
				errlog() << "Invalid string in material file: " << part;
				throw std::exception(std::string("Invalid string in material file: " + part).c_str());
			}

			material->setParameter(match[1], v);
		}

		package.insert({ materialData.first, material });
	}

	for (auto pair : package) {
		m_materialsMap.insert(std::make_pair(pair.first, pair.second));
	}
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