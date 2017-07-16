#pragma once

#include <Engine\Components\GUI\Window.h>

class InputManager {
public:
	InputManager(Window* window);
	~InputManager();

	bool isKeyPressed(int);

private:
	Window* m_window;
};