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

	GraphicsSystem* m_graphicsSystem;
	GraphicsContext* m_graphicsContext;

	InputManager* m_inputMgr;
	ResourceManager* m_resMgr;
	SceneManager* m_sceneMgr;
	GUIManager* m_guiMgr;

	double m_lastFrameTime;
	double m_maxFrameTime;
};