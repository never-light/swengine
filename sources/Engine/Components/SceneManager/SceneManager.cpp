#include "SceneManager.h"

SceneManager::SceneManager() {
	
}

SceneManager::~SceneManager() {
	
}

void SceneManager::initialize(ResourceManager* resourceManager) {
	m_resourceManager = resourceManager;
}

void SceneManager::shutdown() {
	for (auto it = m_scenesMap.begin(); it != m_scenesMap.end(); it++) {
		it->second->shutdown();
		delete it->second;
	}
}

Scene* SceneManager::createEmptyScene(const std::string& name) {
	Scene* scene = new Scene();
	scene->initialize(m_resourceManager);

	m_scenesMap.insert(std::make_pair(name, scene));

	return scene;
}

Scene* SceneManager::getScene(const std::string& name) const {
	return m_scenesMap.at(name);
}