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
	m_lastFrameTime(0),
	m_maxFrameTime(0)
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

		double time = TimeUtils::getCurrentTime();
		render();
		m_lastFrameTime = TimeUtils::getCurrentTime() - time;

		if (m_lastFrameTime > m_maxFrameTime)
			m_maxFrameTime = m_lastFrameTime;

		nextTick += SKIP_TICKS;
		sleepTime = nextTick - GetTickCount();
		if (sleepTime >= 0) {
			Sleep(sleepTime);
		}
	}
}