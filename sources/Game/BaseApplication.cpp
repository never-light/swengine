#include <iostream>
#include <experimental\filesystem>

#include "BaseApplication.h"
#include <Windows.h>

BaseApplication::BaseApplication() 
	: m_window(nullptr), m_renderer(nullptr) {
	
}

BaseApplication::~BaseApplication() {
	shutdown();
}

void BaseApplication::run() {
	const int FRAMES_PER_SECOND = 30;
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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwInit();

	m_window = new Window(windowName, width, height);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
	}

	m_renderer = new Renderer(m_window);

	glfwSetWindowUserPointer(m_window->getWindowPointer(), this);
	glfwSetKeyCallback(m_window->getWindowPointer(), keyCallback);

	m_resMgr = new ResourceManager();
	m_inputMgr = new InputManager(m_window);
}

void BaseApplication::shutdown() {
	glfwTerminate();

	sgFree(m_inputMgr);
	sgFree(m_resMgr);

	sgFree(m_renderer);
	sgFree(m_window);
}

void BaseApplication::onKeyPress(int key) {
	if (key == GLFW_KEY_ESCAPE) {
		m_window->close();
	}
}

void BaseApplication::update() {
	
}

void BaseApplication::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	BaseApplication* app = reinterpret_cast<BaseApplication*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS) {
		app->onKeyPress(key);
	}
}