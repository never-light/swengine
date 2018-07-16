#include "Scene.h"

Scene::Scene(GraphicsResourceFactory* graphicsResourceFactory)
	: m_graphicsResourceFactory(graphicsResourceFactory), m_maxSceneObjectId(0)
{
}

Scene::~Scene() {
	for (auto it = m_cameras.begin(); it != m_cameras.end(); it++) {
		delete it->second;
	}

	for (auto it = m_lights.begin(); it != m_lights.end(); it++) {
		delete it->second;
	}
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

void Scene::registerSceneObject(SceneObject* sceneObject)
{
	assert(sceneObject->getId() == -1);

	sceneObject->setId(generateSceneObjectId());
	m_objects.insert({ sceneObject->getId(), sceneObject });
}

SceneObject* Scene::findSceneObject(SceneObjectId id) {
	return m_objects[id];
}

SceneObject* Scene::findSceneObject(const std::string& name) {
	for (const auto& it : m_objects)
		if (it.second->getName() == name)
			return it.second;
}

void Scene::removeSceneObject(SceneObject* object) {
	m_objects.erase(object->getId());
}

SceneObjectId Scene::generateSceneObjectId() {
	m_maxSceneObjectId++;

	return m_maxSceneObjectId;
}


Light* Scene::createLight(const std::string& name, Light::Type type) {
	Light* lightSource = new Light(type);
	m_lights.insert({ name, lightSource });

	return lightSource;
}

Light* Scene::getLight(const std::string& name) {
	return m_lights[name];
}