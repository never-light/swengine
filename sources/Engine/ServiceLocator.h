#pragma once

#include <Engine\Components\InputManager\InputManager.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>
#include <Engine\Components\SceneManager\SceneManager.h>
#include <Engine\Components\Graphics\GraphicsManager.h>

class ServiceLocator {
public:
	static void registerInputManager(InputManager* inputManager);
	static void registerResourceManager(ResourceManager* resourceManager);
	static void registerSceneManager(SceneManager* sceneManager);
	static void registerGraphicsManager(GraphicsManager* graphicsManager);

	static InputManager* getInputManager();
	static ResourceManager* getResourceManager();
	static SceneManager* getSceneManager();
	static GraphicsManager* getGraphicsManager();

	static void destroyAllServices();

private:
	static InputManager* m_inputManager;
	static ResourceManager* m_resourceManager;
	static SceneManager* m_sceneManager;
	static GraphicsManager* m_graphicsManager;

private:
	ServiceLocator();
	ServiceLocator(const ServiceLocator&);
};