#pragma once

#include <string>
#include <unordered_map>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "Scene.h"

class SceneManager {
public:
	SceneManager();
	~SceneManager();

	void initialize();
	void shutdown();

	Scene* createEmptyScene(const std::string&);
	Scene* getScene(const std::string&) const;

private:
	std::unordered_map<std::string, Scene*> m_scenesMap;
};