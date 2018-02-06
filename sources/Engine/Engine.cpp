#include "Engine.h"
#include <Engine\Components\Graphics\OpenGL3\OpenGL3Renderer.h>

void InitializeEngine(Window* window) {
	ServiceLocator::registerConfigManager(new ConfigManager());
	ServiceLocator::registerInputManager(new InputManager(window));
	ServiceLocator::registerGraphicsManager(new GraphicsManager(window, GraphicsDriver::OpenGL33));
	ServiceLocator::registerResourceManager(new ResourceManager());
	ServiceLocator::registerSceneManager(new SceneManager());
}

void ShutdownEngine() {
	ServiceLocator::destroyAllServices();
}