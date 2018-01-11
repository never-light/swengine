#pragma once

#include <Engine\Components\InputManager\InputManager.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>
#include <Engine\Components\SceneManager\SceneManager.h>
#include <Engine\Components\Graphics\Graphics.h>

class ServiceLocator {
public:
	static void registerInputManager(InputManager* inputManager);
	static void registerResourceManager(ResourceManager* resourceManager);
	static void registerSceneManager(SceneManager* sceneManager);
	static void registerRenderer(Renderer* renderer);

	static InputManager* getInputManager();
	static ResourceManager* getResourceManager();
	static SceneManager* getSceneManager();
	static Renderer* getRenderer();

	static void destroyAllServices();

private:
	static InputManager* m_inputManager;
	static ResourceManager* m_resourceManager;
	static SceneManager* m_sceneManager;
	static Renderer* m_renderer;

private:
	ServiceLocator();
	ServiceLocator(const ServiceLocator&);
};