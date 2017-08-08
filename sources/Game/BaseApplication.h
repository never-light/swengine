#pragma once

#include <string>

#include <Engine\Engine.h>

class BaseApplication {
public:
	BaseApplication();
	virtual ~BaseApplication();

	virtual void run();
	virtual void initialize(const std::string&, unsigned int, unsigned int);
	virtual void shutdown();
	
	virtual void update();
	virtual void render();

protected:
	Window* m_window;
	Renderer* m_renderer;

	InputManager* m_inputMgr;
	ResourceManager* m_resMgr;
	SceneManager* m_sceneMgr;
};