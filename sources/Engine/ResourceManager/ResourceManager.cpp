#include "ResourceManager.h"

ResourceManager::ResourceManager() {
	
}

ResourceManager::~ResourceManager() {
	
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
	sprite->setSize({sprite->getTexture()->getWidth(), sprite->getTexture()->getHeight()});

	m_spritesMap.insert(std::make_pair(filename, sprite));

	return sprite;
}