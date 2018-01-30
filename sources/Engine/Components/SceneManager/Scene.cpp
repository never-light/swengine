#include "Scene.h"
#include <Engine\ServiceLocator.h>

Scene::Scene() 
	: m_rootSceneNode(new SceneNode())
{
	m_rootSceneNode->setName("root");
	m_rootSceneNode->setParentSceneNode(nullptr);
}

Scene::~Scene() {
	for (auto it = m_modelsMap.begin(); it != m_modelsMap.end(); it++) {
		delete it->second;
	}

	for (auto it = m_camerasMap.begin(); it != m_camerasMap.end(); it++) {
		delete it->second;
	}

	for (auto it = m_lightsMap.begin(); it != m_lightsMap.end(); it++) {
		delete it->second;
	}
}

void Scene::setActiveCamera(const std::string& name) {
	m_activeCamera = getCamera(name);
}

void Scene::setActiveCamera(Camera* camera) {
	m_activeCamera = camera;
}

Camera* Scene::getActiveCamera() const {
	return m_activeCamera;
}

Camera* Scene::createCamera(const std::string& name) {
	Camera* camera = new Camera();
	m_camerasMap.insert(std::make_pair(name, camera));

	return camera;
}

Camera* Scene::getCamera(const std::string& name) {
	return m_camerasMap.at(name);
}

Model* Scene::createModel(const std::string& filename, const std::string& name) {
	if (m_modelsMap.find(name) != m_modelsMap.end()) {
		return m_modelsMap.at(name);
	}

	Mesh* mainMesh = ServiceLocator::getResourceManager()->loadMesh(filename);

	Model* model = new Model;

	for (Mesh* subMesh : mainMesh->getSubMeshesArray()) {
		SubModel* subModel = new SubModel(model);
		subModel->setMesh(subMesh);
		model->addSubModel(subModel);
	}

	m_modelsMap.insert(std::make_pair(name, model));

	return model;
}

Model* Scene::getModel(const std::string& name) {
	return m_modelsMap.at(name);
}

SceneNode* Scene::getRootSceneNode() const {
	return m_rootSceneNode;
}

Light* Scene::createLight(const std::string& name, LightType type) {
	Light* lightSource = ServiceLocator::getRenderer()->createLight(type);
	m_lightsMap.insert(std::make_pair(name, lightSource));

	return lightSource;
}

Light* Scene::getLight(const std::string& name) {
	return m_lightsMap.at(name);
}