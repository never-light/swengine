#include "SceneManager.h"

SceneManager::SceneManager() {
	
}

SceneManager::~SceneManager() {
	for (auto it = m_scenesMap.begin(); it != m_scenesMap.end(); it++) {
		delete it->second;
	}
}

Scene* SceneManager::createEmptyScene(const std::string& name) {
	Scene* scene = new Scene();

	m_scenesMap.insert(std::make_pair(name, scene));

	return scene;
}

Scene* SceneManager::getScene(const std::string& name) const {
	return m_scenesMap.at(name);
}