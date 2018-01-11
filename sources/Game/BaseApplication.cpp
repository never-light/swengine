#include "BaseApplication.h"

#include <iostream>
#include <experimental\filesystem>
#include <Windows.h>
#include "config.h"

#include <Engine\Components\Graphics\OpenGL3\OpenGL3Renderer.h>

BaseApplication::BaseApplication(const std::string& windowName, unsigned int width, unsigned int height)
	: m_window(nullptr), 
	m_renderer(nullptr), 
	m_inputMgr(nullptr),
	m_sceneMgr(nullptr)
{
	using std::experimental::filesystem::path;
	using std::experimental::filesystem::current_path;

	current_path(current_path().parent_path());

	Random::initialize();

	// Window
	m_window = new Window(windowName, width, height);
	m_window->setCursorType(CursorType::Hidden);

	// Engine
	InitializeEngine(m_window);

	// Renderer
	m_renderer = ServiceLocator::getRenderer();

	// Input Manager
	m_inputMgr = ServiceLocator::getInputManager();

	// Resource Manager
	m_resMgr = ServiceLocator::getResourceManager();

	// Scene Managaer
	m_sceneMgr = ServiceLocator::getSceneManager();
}

BaseApplication::~BaseApplication() {
	delete m_window;

	ShutdownEngine();
	glfwTerminate();
}

void BaseApplication::update() {
	m_window->update();
	m_inputMgr->update();
}

void BaseApplication::render() {
	m_window->render();
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
		render();

		nextTick += SKIP_TICKS;
		sleepTime = nextTick - GetTickCount();
		if (sleepTime >= 0) {
			Sleep(sleepTime);
		}
	}
}