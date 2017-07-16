#include "BaseApplication.h"

#include <iostream>
#include <experimental\filesystem>
#include <Windows.h>
#include "../config.h"

BaseApplication::BaseApplication() 
	: m_window(nullptr), m_renderer(nullptr) {
	
}

BaseApplication::~BaseApplication() {

}

void BaseApplication::run() {
	const int FRAMES_PER_SECOND = GAME_SPEED;
	const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

	unsigned long nextTick = GetTickCount();

	int sleepTime = 0;

	while (true) {
		if (m_window->shouldClose()) {
			break;
		}

		glfwPollEvents();
		update();
		draw();

		nextTick += SKIP_TICKS;
		sleepTime = nextTick - GetTickCount();
		if (sleepTime >= 0) {
			Sleep(sleepTime);
		}
	}
}

void BaseApplication::draw() {
	m_renderer->beginRendering({ 0.1f, 0.5f, 0.5f, 1.0f });

	m_renderer->endRendering();
}

void BaseApplication::initialize(const std::string& windowName, unsigned int width, unsigned int height) {
	using std::experimental::filesystem::path;
	using std::experimental::filesystem::current_path;

	current_path(current_path().parent_path());

	Random::initialize();

	m_window = new Window(windowName, width, height);

	m_renderer = new Renderer(m_window);
	m_renderer->setProjectionMatrix(glm::ortho(0.0f, static_cast<float>(WIDTH_VALUE), static_cast<float>(HEIGHT_VALUE), 0.0f));

	glfwSetWindowUserPointer(m_window->getWindowPointer(), this);
	glfwSetKeyCallback(m_window->getWindowPointer(), keyCallback);

	m_resMgr = new ResourceManager();
	m_inputMgr = new InputManager(m_window);
}

void BaseApplication::shutdown() {
	delete m_inputMgr;
	delete m_resMgr;

	delete m_renderer;
	delete m_window;

	glfwTerminate();
}

void BaseApplication::onKeyPress(int key) {

}

void BaseApplication::update() {
	
}

void BaseApplication::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	BaseApplication* app = reinterpret_cast<BaseApplication*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		app->onKeyPress(key);
	}
}