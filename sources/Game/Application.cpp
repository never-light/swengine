#include "Application.h"
#include "config.h"

Application::Application() : BaseApplication(), InputEventsListener() {

}

Application::~Application() {
	
}

void Application::initialize(const std::string& windowName, unsigned int width, unsigned int height) {
	infolog() << "Инициализация начата...";
	BaseApplication::initialize(windowName, width, height);

	// Camera Controller
	cameraController = new CameraController();
	cameraController->initialize(m_inputMgr, m_renderer);

	m_inputMgr->registerEventListener(this);

	// Scene
	m_sceneMgr = new SceneManager();
	m_sceneMgr->initialize(m_resMgr);
	m_mainScene = m_sceneMgr->createEmptyScene("main");

	// Create directional light
	Light* light = m_mainScene->createLightSource<OpenGL3Light>("sunLight");
	light->setType(LightType::Directional);
	light->setDirection({ 0, 0, -1 });
	light->setAmbientColor({ 0.2f, 0.2f, 0.2f });
	light->setDuffuseColor({ 1.0f, 1.0f, 1.0f });
	light->setSpecularColor({ 1.0f, 1.0f, 1.0f });
	
	m_renderer->addLight(light);
	
	// Create bronze material
	m_bronzeMaterial = m_resMgr->createEmptyMaterial("bronze");
	m_bronzeMaterial->setGpuProgram(m_resMgr->loadShader("resources/shaders/diffuse.sh"));
	m_bronzeMaterial->setAmbientColor({ 0.2125, 0.1275, 0.054 });
	m_bronzeMaterial->setDuffuseColor({ 0.714, 0.4284, 0.18144 });
	m_bronzeMaterial->setSpecularColor({ 0.393548, 0.271906, 0.166721 });
	m_bronzeMaterial->setShininess(0.2f * 128.0f);

	// Load teapot model
	auto teapot = m_mainScene->createModel("resources/models/teapot.model", "teapot");
	teapot->setMaterial(m_bronzeMaterial);

	// Create teapot node
	SceneNode* terrainNode = m_mainScene->getRootSceneNode()->createChildSceneNode("teapot");
	terrainNode->setPosition({0, -1, 0});
	terrainNode->setScale({1.0f, 1.0f, 1.0f});
	terrainNode->attachObject(teapot);

	infolog() << "Инициализация завершена...";
}

void Application::shutdown() {
	infolog() << "Завершение работы...";

	cameraController->shutdown();
	delete cameraController;

	BaseApplication::shutdown();
}

void Application::update() {
	cameraController->update();
	BaseApplication::update();
}

void Application::render() {
	BaseApplication::render();

	m_renderer->beginRendering({ 0.6f, 0.6f, 0.6f });
	m_renderer->drawModel(m_mainScene->getModel("teapot"));
	m_renderer->endRendering();
}

void Application::onKeyPress(int16 key, KeyEvent::Modifier mod) {
	if (key == GLFW_KEY_ESCAPE) {
		m_window->close();
	}
}