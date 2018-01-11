#pragma once

#include <vector>

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>

#include <Engine\Components\GUI\Window.h>
#include "InputEvents.h"
#include "InputEventsListener.h"

class InputManager {
public:
	InputManager(Window* window);
	~InputManager();

	void update();

	void registerEventListener(InputEventsListener* listener);
	void unregisterEventListener(InputEventsListener* listener);

	KeyState getKeyState(int16 key) const;
	bool isKeyPressed(int16 key) const;
	bool isKeyReleased(int16 key) const;
	bool isKeyRepeated(int16 key) const;

	MousePosition getMousePosition() const;
	MouseButtonState getMouseButtonState(int16 key) const;
	MouseState getMouseState() const;
private:
	void onKeyEvent(int key, int scancode, int action, int mods);
	void onMouseMovedEvent(real64 x, real64 y);
	void onMouseButtonEvent(int button, int action, int mods);
	void onScrollEvent(real64 offsetX, real64 offsetY);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseMovedCallback(GLFWwindow* window, real64 x, real64 y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, real64 offsetX, real64 offsetY);
private:
	Window* m_window;

	std::vector<InputEventsListener*> m_eventListeners;
};