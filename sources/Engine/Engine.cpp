#include "Engine.h"
#include <Engine\Components\Graphics\OpenGL3\OpenGL3Renderer.h>

void InitializeEngine(Window* window) {
	ServiceLocator::registerInputManager(new InputManager(window));
	ServiceLocator::registerRenderer(new OpenGL3Renderer(window));
	ServiceLocator::registerResourceManager(new ResourceManager());
	ServiceLocator::registerSceneManager(new SceneManager());
}

void ShutdownEngine() {
	ServiceLocator::destroyAllServices();
}