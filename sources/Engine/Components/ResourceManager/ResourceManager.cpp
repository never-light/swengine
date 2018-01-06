#include "ResourceManager.h"
#include <regex>
#include <Engine\Components\Debugging\Log.h>

#include <Engine\Components\Graphics\OpenGL3\OpenGL3TextureLoader.h>
#include <Engine\Components\Graphics\OpenGL3\OpenGL3GpuProgramLoader.h>
#include <Engine\Components\Graphics\OpenGL3\OpenGL3MaterialLoader.h>
#include <Engine\Components\Graphics\OpenGL3\OpenGL3MeshLoader.h>
#include <Engine\Components\Graphics\OpenGL3\OpenGL3SpriteLoader.h>

ResourceManager::ResourceManager() {
	m_textureLoader = new OpenGL3TextureLoader();
	m_gpuProgramLoader = new OpenGL3GpuProgramLoader();
	m_materialLoader = new OpenGL3MaterialLoader();
	m_meshLoader = new OpenGL3MeshLoader();
	m_spriteLoader = new OpenGL3SpriteLoader();
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

	Texture* texture = m_textureLoader->loadFromFile(filename);
	m_texturesMap.insert(std::make_pair(filename, texture));

	return texture;
}

GpuProgram* ResourceManager::loadShader(const std::string& filename) {
	if (m_shadersMap.find(filename) != m_shadersMap.end()) {
		return m_shadersMap.at(filename);
	}

	GpuProgram* shader = m_gpuProgramLoader->loadFromFile(filename);
	m_shadersMap.insert(std::make_pair(filename, shader));

	return shader;
}

Sprite* ResourceManager::loadSprite(const std::string& filename) {
	if (m_spritesMap.find(filename) != m_spritesMap.end()) {
		return m_spritesMap.at(filename);
	}

	Sprite* sprite = m_spriteLoader->createSprite();
	sprite->setTexture(loadTexture(filename));
	sprite->setShader(loadShader("resources/shaders/sprite.sh"));

	m_spritesMap.insert(std::make_pair(filename, sprite));

	return sprite;
}

Mesh* ResourceManager::loadMesh(const std::string& filename) {
	if (m_meshesMap.find(filename) != m_meshesMap.end()) {
		return m_meshesMap.at(filename);
	}

	Mesh* mesh = m_meshLoader->createMesh();

	std::vector<Mesh*>& meshes = m_meshLoader->loadFromFile(filename);

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
				auto material = m_materialLoader->createMaterial(parameters);
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
				v = MaterialParameterValue(std::atof(std::string(match[2]).c_str()));
			}
			// String value
			else if (std::regex_search(currentLine, match, materialParameterStringRegex)) {
				if (match[1] == "program") {
					v = MaterialParameterValue(loadShader(std::string(match[2])));
				}
				else {
					v = MaterialParameterValue(std::string(match[2]));
				}
			}
			// Color value
			else if (std::regex_search(currentLine, match, materialParameterColorRegex)) {
				v = MaterialParameterValue(Color(
					std::atof(std::string(match[2]).c_str()), // r
					std::atof(std::string(match[3]).c_str()), // g
					std::atof(std::string(match[4]).c_str()) // b
				));
			}
			// Texture value
			else if (std::regex_search(currentLine, match, materialParameterTextureRegex)) {
				v = MaterialParameterValue(loadTexture(match[2]));
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
		auto material = m_materialLoader->createMaterial(parameters);
		package.insert(std::make_pair(currentMaterialName, material));
	}

	for (auto pair : package) {
		m_materialsMap.insert(std::make_pair(pair.first, pair.second));
	}
}

Material* ResourceManager::createMaterial(const std::string& name) {
	Material* material = m_materialLoader->createMaterial();
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