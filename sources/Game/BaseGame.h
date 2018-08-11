#pragma once

#include <string>

#include <Engine\Engine.h>

class BaseGame {
public:
	BaseGame(const std::string& windowName, unsigned int width, unsigned int height);
	virtual ~BaseGame();

	virtual void run();
	virtual void update();
	virtual void render();

protected:
	Window* m_window;

	InputManager* m_inputMgr;
	SceneManager* m_sceneMgr;
};