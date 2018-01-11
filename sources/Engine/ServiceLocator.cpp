#include "ServiceLocator.h"

InputManager* ServiceLocator::m_inputManager = nullptr;
ResourceManager* ServiceLocator::m_resourceManager = nullptr;
SceneManager* ServiceLocator::m_sceneManager = nullptr;
Renderer* ServiceLocator::m_renderer = nullptr;

void ServiceLocator::registerInputManager(InputManager* inputManager) {
	m_inputManager = inputManager;
}

void ServiceLocator::registerResourceManager(ResourceManager* resourceManager) {
	m_resourceManager = resourceManager;
}

void ServiceLocator::registerSceneManager(SceneManager* sceneManager) {
	m_sceneManager = sceneManager;
}

void ServiceLocator::registerRenderer(Renderer* renderer) {
	m_renderer = renderer;
}

InputManager* ServiceLocator::getInputManager() {
	return m_inputManager;
}

ResourceManager* ServiceLocator::getResourceManager() {
	return m_resourceManager;
}

SceneManager* ServiceLocator::getSceneManager() {
	return m_sceneManager;
}

Renderer* ServiceLocator::getRenderer() {
	return m_renderer;
}

void ServiceLocator::destroyAllServices() {
	if (m_inputManager)
		delete m_inputManager;

	if (m_resourceManager)
		delete m_resourceManager;

	if (m_sceneManager)
		delete m_sceneManager;

	if (m_renderer)
		delete m_renderer;
}