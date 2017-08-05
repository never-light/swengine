#include "Scene.h"

Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::initialize() {
	m_rootSceneNode = new SceneNode();
	m_rootSceneNode->setName("root");
	m_rootSceneNode->setParentSceneNode(nullptr);
}

void Scene::shutdown() {
	for (auto it = m_camerasMap.begin(); it != m_camerasMap.end(); it++) {
		delete it->second;
	}

	for (auto it = m_lightsMap.begin(); it != m_lightsMap.end(); it++) {
		delete it->second;
	}
}

Camera* Scene::createCamera(const std::string& name) {
	Camera* camera = new Camera();
	m_camerasMap.insert(std::make_pair(name, camera));

	return camera;
}

Camera* Scene::getCamera(const std::string& name) {
	return m_camerasMap.at(name);
}

SceneNode* Scene::getRootSceneNode() const {
	return m_rootSceneNode;
}