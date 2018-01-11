#pragma once

#include <string>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>
#include "Scene.h"

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	Scene* createEmptyScene(const std::string&);
	Scene* getScene(const std::string&) const;

private:
	std::unordered_map<std::string, Scene*> m_scenesMap;
};