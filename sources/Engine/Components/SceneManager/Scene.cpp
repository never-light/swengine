#include "Scene.h"

Scene::Scene(GraphicsContext* graphicsContext, ResourceManager* resourceManager)
	: m_id(0), 
	m_graphicsContext(graphicsContext), m_resourceManager(resourceManager)
{
}

Scene::~Scene() {
	for (auto it = m_cameras.begin(); it != m_cameras.end(); it++) {
		delete it->second;
	}
}

void Scene::onRegister(SceneId id) {
	m_id = id;
}

void Scene::activate()
{
}

void Scene::deactivate()
{
}

void Scene::setActiveCamera(Camera* camera) {
	m_activeCamera = camera;
}

Camera* Scene::getActiveCamera() const {
	return m_activeCamera;
}

Camera* Scene::createCamera(const std::string& name) {
	Camera* camera = new Camera();
	m_cameras.insert({ name, camera });

	return camera;
}

Camera* Scene::getCamera(const std::string& name) {
	return m_cameras[name];
}

SceneId Scene::getId() const
{
	return m_id;
}