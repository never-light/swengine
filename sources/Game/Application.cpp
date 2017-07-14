#include "Application.h"
#include "config.h"

Application::Application() : BaseApplication() {

}

Application::~Application() {
	
}

void Application::initialize(const std::string& windowName, unsigned int width, unsigned int height) {
	infolog() << "Инициализация...";
	BaseApplication::initialize(windowName, width, height);

	m_stateMgr = new StateManager();
	m_stateMgr->registerState(STATE_MAIN_MENU, m_resMgr);
	m_stateMgr->registerState(STATE_PLAY, m_resMgr);

	m_stateMgr->changeState(STATE_MAIN_MENU);

	infolog() << "Инициализация завершена...";
}

void Application::draw() {
	m_renderer->beginRendering({ 0.0f, 0.0f, 0.0f, 1.0f });

	m_stateMgr->getCurrentState()->draw(m_renderer);

	m_renderer->endRendering();
}

void Application::onKeyPress(int key) {
	BaseApplication::onKeyPress(key);

	if (key == GLFW_KEY_ESCAPE) {
		stateId newState = (m_stateMgr->getCurrentStateId() == STATE_PLAY) ? STATE_MAIN_MENU : STATE_PLAY;
		m_stateMgr->changeState(newState);
	}
}

void Application::shutdown() {
	infolog() << "Завершение работы...";

	m_stateMgr->unregisterState(STATE_PLAY, m_resMgr);
	m_stateMgr->unregisterState(STATE_MAIN_MENU, m_resMgr);

	BaseApplication::shutdown();
}

void Application::update() {
	m_stateMgr->update();
	m_stateMgr->getCurrentState()->update(m_inputMgr);
}