#include "ResourceManager.h"

ResourceManager::ResourceManager() {
	
}

ResourceManager::~ResourceManager() {
	{
		auto itr = m_meshesMap.begin();
		while (itr != m_meshesMap.end()) {
			delete itr->second;
			itr = m_meshesMap.erase(itr);
		}
	}

	{
		auto itr = m_spritesMap.begin();
		while (itr != m_spritesMap.end()) {
			delete itr->second;
			itr = m_spritesMap.erase(itr);
		}
	}
	{
		auto itr = m_shadersMap.begin();
		while (itr != m_shadersMap.end()) {
			delete itr->second;
			itr = m_shadersMap.erase(itr);
		}
	}
	{
		auto itr = m_texturesMap.begin();
		while (itr != m_texturesMap.end()) {
			delete itr->second;
			itr = m_texturesMap.erase(itr);
		}
	}
}

Texture* ResourceManager::loadTexture(const std::string& filename) {
	if (m_texturesMap.find(filename) != m_texturesMap.end()) {
		return m_texturesMap.at(filename);
	}

	Texture* texture = new Texture;
	texture->loadFromFile(filename);

	m_texturesMap.insert(std::make_pair(filename, texture));

	return texture;
}

Shader* ResourceManager::loadShader(const std::string& filename) {
	if (m_shadersMap.find(filename) != m_shadersMap.end()) {
		return m_shadersMap.at(filename);
	}

	Shader* shader = new Shader;
	shader->loadFromFile(filename);

	m_shadersMap.insert(std::make_pair(filename, shader));

	return shader;
}

Sprite* ResourceManager::loadSprite(const std::string& filename) {
	if (m_spritesMap.find(filename) != m_spritesMap.end()) {
		return m_spritesMap.at(filename);
	}

	Sprite* sprite = new Sprite();
	sprite->setTexture(loadTexture(filename));
	sprite->setShader(loadShader("resources/shaders/sprite.sh"));

	m_spritesMap.insert(std::make_pair(filename, sprite));

	return sprite;
}

Mesh* ResourceManager::loadMesh(const std::string& filename) {
	if (m_meshesMap.find(filename) != m_meshesMap.end()) {
		return m_meshesMap.at(filename);
	}

	Mesh* mesh = new Mesh();

	MeshLoader loader;
	std::vector<Mesh*>& meshes = loader.load(filename);

	for (Mesh* subMesh : meshes) {
		mesh->addSubMesh(subMesh);
	}

	m_meshesMap.insert(std::make_pair(filename, mesh));

	return mesh;
}