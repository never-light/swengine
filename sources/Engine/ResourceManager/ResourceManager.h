#pragma once

#include <string>
#include <map>

#include "../Renderer/OpenGL3Texture.h"
#include "../Renderer/OpenGL3Shader.h"
#include "../Renderer/OpenGL3Sprite.h"


class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	Sprite* loadSprite(const std::string&);
	Texture* loadTexture(const std::string&);
	Shader* loadShader(const std::string&);
private:
	std::map<std::string, Texture*> m_texturesMap;
	std::map<std::string, Shader*> m_shadersMap;
	std::map<std::string, Sprite*> m_spritesMap;
};