#include "SceneManager.h"

SceneManager::SceneManager(GraphicsResourceFactory* graphicsResourceFactory) 
	: m_graphicsResourceFactory(graphicsResourceFactory)
{
	
}

SceneManager::~SceneManager() {
	for (auto it = m_scenes.begin(); it != m_scenes.end(); it++)
		delete it->second;
}

Scene* SceneManager::createScene(const std::string& name) {
	Scene* scene = new Scene(m_graphicsResourceFactory);

	m_scenes.insert(std::make_pair(name, scene));

	return scene;
}

Scene* SceneManager::getScene(const std::string& name) const {
	return m_scenes.at(name);
}