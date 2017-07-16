#pragma once

#include <string>
#include <map>

#include <Engine\Components\Graphics\Texture.h>
#include <Engine\Components\Graphics\Shader.h>
#include <Engine\Components\Graphics\Sprite.h>

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