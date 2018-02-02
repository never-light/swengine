#include "ServiceLocator.h"

InputManager* ServiceLocator::m_inputManager = nullptr;
ResourceManager* ServiceLocator::m_resourceManager = nullptr;
SceneManager* ServiceLocator::m_sceneManager = nullptr;
GraphicsManager* ServiceLocator::m_graphicsManager = nullptr;

void ServiceLocator::registerInputManager(InputManager* inputManager) {
	m_inputManager = inputManager;
}

void ServiceLocator::registerResourceManager(ResourceManager* resourceManager) {
	m_resourceManager = resourceManager;
}

void ServiceLocator::registerSceneManager(SceneManager* sceneManager) {
	m_sceneManager = sceneManager;
}

void ServiceLocator::registerGraphicsManager(GraphicsManager* graphicsManager) {
	m_graphicsManager = graphicsManager;
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

GraphicsManager* ServiceLocator::getGraphicsManager() {
	return m_graphicsManager;
}

void ServiceLocator::destroyAllServices() {
	if (m_inputManager)
		delete m_inputManager;

	if (m_resourceManager)
		delete m_resourceManager;

	if (m_sceneManager)
		delete m_sceneManager;

	if (m_graphicsManager)
		delete m_graphicsManager;
}