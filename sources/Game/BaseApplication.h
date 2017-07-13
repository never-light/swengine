#pragma once

#include <string>

#include "../Engine/Engine.h"

class BaseApplication {
public:
	BaseApplication();
	virtual ~BaseApplication();

	virtual void run();
	virtual void initialize(const std::string&, unsigned int, unsigned int);
	virtual void shutdown();
	
protected:
	virtual void update();
	virtual void draw();
	virtual void onKeyPress(int);

private:
	static void keyCallback(GLFWwindow*, int, int, int, int);

protected:
	Window* m_window;
	Renderer* m_renderer;

	ResourceManager* m_resMgr;
	InputManager* m_inputMgr;
};