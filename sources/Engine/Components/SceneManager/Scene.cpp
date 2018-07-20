#include "Scene.h"

Scene::Scene(GraphicsContext* graphicsContext, ResourceManager* resourceManager)
	: m_id(0), m_maxSceneObjectId(0), 
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

void Scene::render()
{
	for (auto& it : m_objects)
		it.second->render(m_activeCamera);
}

void Scene::update()
{
	for (auto& it : m_objects)
		it.second->update();
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

SceneObjectId Scene::registerSceneObject(SceneObject* sceneObject)
{
	assert(sceneObject->getId() == 0);

	sceneObject->onRegister(generateSceneObjectId());
	m_objects.insert({ sceneObject->getId(), sceneObject });

	return sceneObject->getId();
}

SceneObject* Scene::findSceneObject(SceneObjectId id) {
	return m_objects[id];
}

SceneObject* Scene::findSceneObject(const std::string& name) {
	for (const auto& it : m_objects)
		if (it.second->getName() == name)
			return it.second;

	return nullptr;
}

void Scene::removeSceneObject(SceneObject* object) {
	m_objects.erase(object->getId());
}

SceneId Scene::getId() const
{
	return m_id;
}

SceneObjectId Scene::generateSceneObjectId() {
	m_maxSceneObjectId++;

	return m_maxSceneObjectId;
}