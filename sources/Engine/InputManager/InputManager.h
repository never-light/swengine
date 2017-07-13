#pragma once

#include "../Renderer/OpenGL3Window.h"

class InputManager {
public:
	InputManager(Window* window);
	~InputManager();

	bool isKeyPressed(int);

private:
	Window* m_window;
};