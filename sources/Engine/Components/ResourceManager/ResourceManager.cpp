#include "ResourceManager.h"

#include <regex>
#include <fstream>
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

	Texture* texture = ServiceLocator::getRenderer()->createTexture(width, height, data);
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

	GpuProgram* program = ServiceLocator::getRenderer()->createGpuProgram(source);
	m_shadersMap.insert(std::make_pair(filename, program));

	return program;
}

Sprite* ResourceManager::loadSprite(const std::string& filename) {
	if (m_spritesMap.find(filename) != m_spritesMap.end()) {
		return m_spritesMap.at(filename);
	}

	Sprite* sprite = ServiceLocator::getRenderer()->createSprite(loadTexture(filename), loadShader("resources/shaders/sprite.sh"));
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
		Mesh* mesh = ServiceLocator::getRenderer()->createMesh();

		ModelFileMeshData meshData;
		in.read((char*)&meshData, sizeof meshData);

		for (size_t j = 0; j < meshData.verticesCount; j++) {
			ModelFileVertexData vertexData;
			in.read((char*)&vertexData, sizeof vertexData);

			mesh->addVertex(Vertex(
				vector3(vertexData.x, vertexData.y, vertexData.z),
				vector2(vertexData.u, vertexData.v),
				vector3(vertexData.nx, vertexData.ny, vertexData.nz)
			));
		}

		for (size_t j = 0; j < meshData.indicesCount; j++) {
			uint32 index;
			in.read((char*)&index, sizeof index);

			mesh->addIndex(index);
		}

		mesh->setName(meshData.name);
		mesh->updateState();

		meshes.push_back(mesh);
	}

	Mesh* mesh = ServiceLocator::getRenderer()->createMesh();
	for (Mesh* subMesh : meshes) {
		mesh->addSubMesh(subMesh);
	}

	m_meshesMap.insert(std::make_pair(filename, mesh));

	return mesh;
}

void ResourceManager::loadMaterialsPackage(const std::string& filename) {
	std::unordered_map<std::string, Material*> package;

	std::ifstream in(filename);
	std::string currentLine, currentMaterialName;

	MaterialParametersList parameters;

	std::regex materialNameRegex("^\\[([a-zA-Z0-9]+)\\]$");
	std::regex materialParameterNumberRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+([-+]?[0-9]*\\.?[0-9]+)$");
	std::regex materialParameterStringRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+\\\"([0-9a-zA-Z\\.\\\\\\/_]+)\\\"$");
	std::regex materialParameterColorRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+color\\(([-+]?[0-9]*\\.?[0-9]+),[\\s]+([-+]?[0-9]*\\.?[0-9]+),[\\s]+([-+]?[0-9]*\\.?[0-9]+)\\)$");
	std::regex materialParameterTextureRegex("^([a-zA-Z]+)[\\s]+[=][\\s]+texture\\(\\\"([0-9a-zA-Z\\.\\\\\\/_]+)\\\"\\)$");

	std::smatch match;

	while (std::getline(in, currentLine)) {
		if (currentLine.empty()) {
			continue;
		}

		if (std::regex_search(currentLine, match, materialNameRegex)) {
			if (!currentMaterialName.empty()) {
				// Save material
				auto material = createMaterialWithParameters(parameters);
				package.insert(std::make_pair(currentMaterialName, material));

				currentMaterialName.clear();
				parameters.clear();
			}

			currentMaterialName = match[1];
		}
		else if (!currentMaterialName.empty()) {
			MaterialParameterValue v;

			// Number Value
			if (std::regex_search(currentLine, match, materialParameterNumberRegex)) {
				v = std::atof(std::string(match[2]).c_str());
			}
			// String value
			else if (std::regex_search(currentLine, match, materialParameterStringRegex)) {
				if (match[1] == "program") {
					v = loadShader(std::string(match[2]));
				}
				else {
					v = std::string(match[2]);
				}
			}
			// Color value
			else if (std::regex_search(currentLine, match, materialParameterColorRegex)) {
				v = Color(
					std::atof(std::string(match[2]).c_str()), // r
					std::atof(std::string(match[3]).c_str()), // g
					std::atof(std::string(match[4]).c_str()) // b
				);
			}
			// Texture value
			else if (std::regex_search(currentLine, match, materialParameterTextureRegex)) {
				v = loadTexture(match[2]);
			}
			else {
				errlog() << "Invalid string in material file: " << currentLine;
				throw std::exception(std::string("Invalid string in material file: " + currentLine).c_str());
			}

			parameters.insert(std::make_pair(
				match[1], // Parameter name
				v // value
			));
		}
	}

	// Save last material
	if (!currentMaterialName.empty()) {
		auto material = createMaterialWithParameters(parameters);
		package.insert(std::make_pair(currentMaterialName, material));
	}

	for (auto pair : package) {
		m_materialsMap.insert(std::make_pair(pair.first, pair.second));
	}
}

Material* ResourceManager::createMaterial(const std::string& name) {
	Material* material = ServiceLocator::getRenderer()->createMaterial();
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

Material* ResourceManager::createMaterialWithParameters(const MaterialParametersList& parameters) {
	Material* material = ServiceLocator::getRenderer()->createMaterial();

	setMaterialParameterIfExists(material, parameters, "program");
	setMaterialParameterIfExists(material, parameters, "ambientColor");
	setMaterialParameterIfExists(material, parameters, "diffuseColor");
	setMaterialParameterIfExists(material, parameters, "specularColor");
	setMaterialParameterIfExists(material, parameters, "specularLevel");
	setMaterialParameterIfExists(material, parameters, "diffuseMap");
	setMaterialParameterIfExists(material, parameters, "specularMap");

	return material;
}

void ResourceManager::setMaterialParameterIfExists(Material* material, const MaterialParametersList& parameters, const std::string& parameterName) {
	auto it = parameters.find(parameterName);

	if (it == parameters.end()) {
		return;
	}

	if (parameterName == "ambientColor") {
		material->setAmbientColor(std::get<Color>(it->second));
	}
	else if (parameterName == "diffuseColor") {
		material->setDuffuseColor(std::get<Color>(it->second));
	}
	else if (parameterName == "specularColor") {
		material->setSpecularColor(std::get<Color>(it->second));
	}
	else if (parameterName == "specularLevel") {
		material->setShininess(std::get<real>(it->second));
	}
	else if (parameterName == "diffuseMap") {
		material->setLightmap(LightmapType::Diffuse, std::get<Texture*>(it->second));
	}
	else if (parameterName == "specularMap") {
		material->setLightmap(LightmapType::Specular, std::get<Texture*>(it->second));
	}
	else if (parameterName == "program") {
		material->setGpuProgram(std::get<GpuProgram*>(it->second));
	}
}