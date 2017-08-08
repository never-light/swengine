#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>

#include <Engine\types.h>
#include <Engine\Components\Graphics\Texture.h>
#include <Engine\Components\Graphics\Shader.h>
#include <Engine\Components\Graphics\Sprite.h>
#include <Engine\Components\Graphics\Model.h>

#include "MeshLoader.h"

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();

	Sprite* loadSprite(const std::string&);
	Texture* loadTexture(const std::string&);
	Shader* loadShader(const std::string&);
	Mesh* loadMesh(const std::string&);
private:
	std::map<std::string, Texture*> m_texturesMap;
	std::map<std::string, Shader*> m_shadersMap;
	std::map<std::string, Sprite*> m_spritesMap;
	std::map<std::string, Mesh*> m_meshesMap;
};