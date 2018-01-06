#include "BaseApplication.h"

#include <iostream>
#include <experimental\filesystem>
#include <Windows.h>
#include "config.h"

#include <Engine\Components\Graphics\OpenGL3\OpenGL3Renderer.h>

BaseApplication::BaseApplication() 
	: m_window(nullptr), 
	m_renderer(nullptr), 
	m_inputMgr(nullptr),
	m_sceneMgr(nullptr)
{
	
}

BaseApplication::~BaseApplication() {

}

void BaseApplication::initialize(const std::string& windowName, unsigned int width, unsigned int height) {
	using std::experimental::filesystem::path;
	using std::experimental::filesystem::current_path;

	current_path(current_path().parent_path());

	Random::initialize();

	// Window
	m_window = new Window();
	m_window->initialize(windowName, width, height);
	m_window->setCursorType(CursorType::Hidden);

	// Renderer
	m_renderer = new OpenGL3Renderer(m_window);

	// Input Manager
	m_inputMgr = new InputManager();
	m_inputMgr->initialize(m_window);

	// Resource Manager
	m_resMgr = new ResourceManager();

	// Scene Managaer
	m_sceneMgr = new SceneManager();
	m_sceneMgr->initialize(m_resMgr);
}

void BaseApplication::shutdown() {
	m_sceneMgr->shutdown();
	delete m_sceneMgr;

	delete m_resMgr;

	m_inputMgr->shutdown();
	delete m_inputMgr;

	delete m_renderer;

	m_window->shutdown();
	delete m_window;

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