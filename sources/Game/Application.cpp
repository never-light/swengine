#include "Application.h"
#include "config.h"

Application::Application() : BaseApplication() {

}

Application::~Application() {
	
}

void Application::initialize(const std::string& windowName, unsigned int width, unsigned int height) {
	infolog() << "Инициализация...";
	BaseApplication::initialize(windowName, width, height);

	m_renderer->setProjectionMatrix(glm::ortho(0.0f, static_cast<float>(WIDTH_VALUE), static_cast<float>(HEIGHT_VALUE), 0.0f));

	m_playerSpaceshipController.initialize(m_resMgr);
	m_asteroidsController.initialize(m_resMgr);

	infolog() << "Инициализация завершена...";
}

void Application::draw() {
	m_renderer->beginRendering({ 0.0f, 0.0f, 0.0f, 1.0f });

	m_playerSpaceshipController.draw(m_renderer);
	m_asteroidsController.draw(m_renderer);

	m_renderer->endRendering();
}

void Application::onKeyPress(int key) {
	BaseApplication::onKeyPress(key);
}

void Application::shutdown() {
	infolog() << "Завершение работы...";

	m_playerSpaceshipController.shutdown();
	m_asteroidsController.shutdown();

	BaseApplication::shutdown();
}

void Application::update() {
	m_playerSpaceshipController.update(m_inputMgr);
	m_asteroidsController.update(m_inputMgr);
}