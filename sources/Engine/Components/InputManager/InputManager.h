#pragma once

#include <vector>

#include <Engine/types.h>
#include <Engine/Components/Math/Math.h>

#include <Engine/Components/GUI/Window.h>
#include "InputEvents.h"
#include "InputEventsListener.h"

class InputManager {
public:
	InputManager(Window* window);
	~InputManager();

	void update();

	void registerEventListener(InputEventsListener* listener);
	void unregisterEventListener(InputEventsListener* listener);

	KeyState getKeyState(Key key) const;
	bool isKeyPressed(Key key) const;
	bool isKeyReleased(Key key) const;
	bool isKeyRepeated(Key key) const;

	MousePosition getMousePosition() const;
	MouseButtonState getMouseButtonState(MouseButton button) const;
	MouseState getMouseState() const;
private:
	void onKeyEvent(int key, int scancode, int action, int mods);
	void onCharacterEnteredEvent(unsigned int character);
	void onMouseMovedEvent(double x, double y);
	void onMouseButtonEvent(int button, int action, int mods);
	void onScrollEvent(double offsetX, double offsetY);

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void charCallback(GLFWwindow* window, unsigned int codepoint);
	static void mouseMovedCallback(GLFWwindow* window, double x, double y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double offsetX, double offsetY);
private:
	Window* m_window;

	std::vector<InputEventsListener*> m_eventListeners;
};