#include "BaseGame.h"

#include <iostream>
#include <experimental\filesystem>
#include <Windows.h>
#include "config.h"

#include <Engine\Components\Math\Random.h>
#include <Engine\Utils\time.h>

BaseGame::BaseGame(const std::string& windowName, unsigned int width, unsigned int height)
	: m_window(nullptr), 
	m_inputMgr(nullptr),
	m_sceneMgr(nullptr),
	m_updateTime(0.0),
	m_renderTime(0.0)
{
	using std::experimental::filesystem::path;
	using std::experimental::filesystem::current_path;

	current_path(current_path().parent_path());

	Random::initialize();

	// Window
	m_window = new Window(windowName, width, height, false);
	m_window->setCursorMode(CursorMode::UnlimitedHidden);

	// Engine
	InitializeEngine(m_window);

	// Input Manager
	m_inputMgr = new InputManager(m_window);

	// Scene Managaer
	m_sceneMgr = new SceneManager();
}

BaseGame::~BaseGame() {
	delete m_sceneMgr;
	delete m_inputMgr;

	delete m_window;

	ShutdownEngine();
	glfwTerminate();
}

void BaseGame::update() {
}

void BaseGame::render() {

}

void BaseGame::run() {
	const int FRAMES_PER_SECOND = GAME_STATE_UPDATES_PER_SECOND;
	const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;

	unsigned long nextTick = GetTickCount();

	int sleepTime = 0;


	while (true) {
		double frameStartTime = TimeUtils::getCurrentTime();

		if (m_window->shouldClose()) {
			break;
		}

		glfwPollEvents();
		
		double startTime = TimeUtils::getCurrentTime();
		update();
		m_updateTime = TimeUtils::getCurrentTime() - startTime;

		startTime = TimeUtils::getCurrentTime();
		render();
		m_renderTime = TimeUtils::getCurrentTime() - startTime;

		nextTick += SKIP_TICKS;
		sleepTime = nextTick - GetTickCount();

		m_fullFrameTime = TimeUtils::getCurrentTime() - frameStartTime;

		if (sleepTime >= 0) {
			Sleep(sleepTime);
		}
	}
}