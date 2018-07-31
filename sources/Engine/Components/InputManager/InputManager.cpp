#include "InputManager.h"

InputManager::InputManager(Window* window) : m_window(window) {
	glfwSetWindowUserPointer(m_window->getWindowPointer(), this);
	glfwSetKeyCallback(m_window->getWindowPointer(), keyCallback);
	glfwSetCharCallback(m_window->getWindowPointer(), charCallback);
	glfwSetCursorPosCallback(m_window->getWindowPointer(), mouseMovedCallback);
	glfwSetMouseButtonCallback(m_window->getWindowPointer(), mouseButtonCallback);
	glfwSetScrollCallback(m_window->getWindowPointer(), scrollCallback);
}

InputManager::~InputManager() {

}

void InputManager::update() {

}

void InputManager::registerEventListener(InputEventsListener* listener) {
	m_eventListeners.push_back(listener);
}

void InputManager::unregisterEventListener(InputEventsListener* listener) {
	for (auto it = m_eventListeners.begin(); it != m_eventListeners.end(); it++) {
		if (*it == listener) {
			m_eventListeners.erase(it);
		}
	}
}

KeyState InputManager::getKeyState(Key key) const {
	KeyState state = KeyState::Unknown;

	switch (glfwGetKey(m_window->getWindowPointer(), key)) {
	case GLFW_PRESS:
		state = KeyState::Pressed;
		break;

	case GLFW_RELEASE:
		state = KeyState::Released;
		break;

	case GLFW_REPEAT:
		state = KeyState::Repeated;
		break;
	}

	return state;
}

bool InputManager::isKeyPressed(Key key) const {
	return glfwGetKey(m_window->getWindowPointer(), key) == GLFW_PRESS;
}

bool InputManager::isKeyReleased(Key key) const {
	return glfwGetKey(m_window->getWindowPointer(), key) == GLFW_RELEASE;
}

bool InputManager::isKeyRepeated(Key key) const {
	return glfwGetKey(m_window->getWindowPointer(), key) == GLFW_REPEAT;
}

MousePosition InputManager::getMousePosition() const {
	MousePosition pos;
	glfwGetCursorPos(m_window->getWindowPointer(), &pos.x, &pos.y);
	
	return pos;
}

MouseButtonState InputManager::getMouseButtonState(MouseButton button) const {
	MouseButtonState state = MouseButtonState::Unknown;

	switch (glfwGetMouseButton(m_window->getWindowPointer(), button)) {
	case GLFW_PRESS:
		state = MouseButtonState::Pressed;
		break;

	case GLFW_RELEASE:
		state = MouseButtonState::Released;
		break;
	}

	return state;
}

MouseState InputManager::getMouseState() const {
	MouseState state;

	state.position = getMousePosition();
	state.leftButtonState = getMouseButtonState(GLFW_MOUSE_BUTTON_LEFT);
	state.rightButtonState = getMouseButtonState(GLFW_MOUSE_BUTTON_RIGHT);

	return state;
}

void InputManager::onKeyEvent(int key, int scancode, int action, int mods) {
	KeyEvent::Modifier mod;

	switch (mods) {
	case GLFW_MOD_CONTROL:
		mod = KeyEvent::Modifier::Control;
		break;

	case GLFW_MOD_SHIFT:
		mod = KeyEvent::Modifier::Shift;
		break;

	case GLFW_MOD_ALT:
		mod = KeyEvent::Modifier::Alt;
		break;

	default:
		mod = KeyEvent::Modifier::None;
		break;
	}

	if (action == GLFW_PRESS) {
		for (InputEventsListener* listener : m_eventListeners) {
			listener->onKeyPress(key, mod);
		}
	}
	else if (action == GLFW_RELEASE) {
		for (InputEventsListener* listener : m_eventListeners) {
			listener->onKeyRelease(key, mod);
		}
	}
	else if (action == GLFW_REPEAT) {
		for (InputEventsListener* listener : m_eventListeners) {
			listener->onKeyRepeat(key, mod);
		}
	}
}

void InputManager::onCharacterEnteredEvent(unsigned int character)
{
	unsigned char ch = static_cast<unsigned char>(character);

	if (1040 <= character && character <= 1103) {
		ch = (unsigned char)'À' + (character - 1040);
	}
	else if (character > 255)
		return;

	for (InputEventsListener* listener : m_eventListeners) {
		listener->onCharacterEntered(ch);
	}
}

void InputManager::onMouseMovedEvent(double x, double y) {
	MouseState state;
	state.position.x = x;
	state.position.y = y;

	state.leftButtonState = getMouseButtonState(GLFW_MOUSE_BUTTON_LEFT);
	state.rightButtonState = getMouseButtonState(GLFW_MOUSE_BUTTON_RIGHT);

	for (InputEventsListener* listener : m_eventListeners) {
		listener->onMouseMove(state);
	}
}

void InputManager::onMouseButtonEvent(int button, int action, int mods) {
	MouseState state;
	state.position = getMousePosition();

	MouseButtonState buttonState;

	switch (action) {
	case GLFW_PRESS:
		buttonState = MouseButtonState::Pressed;
		break;

	case GLFW_RELEASE:
		buttonState = MouseButtonState::Released;
		break;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		state.leftButtonState = buttonState;
		state.rightButtonState = getMouseButtonState(GLFW_MOUSE_BUTTON_RIGHT);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		state.leftButtonState = getMouseButtonState(GLFW_MOUSE_BUTTON_LEFT);
		state.rightButtonState = buttonState;
	}

	if (action == GLFW_PRESS) {
		for (InputEventsListener* listener : m_eventListeners) {
			listener->onMouseButtonPress(button, state);
		}
	}
	else if (action == GLFW_RELEASE) {
		for (InputEventsListener* listener : m_eventListeners) {
			listener->onMouseButtonRelease(button, state);
		}
	}
}

void InputManager::onScrollEvent(double offsetX, double offsetY) {
	for (InputEventsListener* listener : m_eventListeners) {
		listener->onScroll(offsetX, offsetY);
	}
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	auto inputManager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
	inputManager->onKeyEvent(key, scancode, action, mods);
}

void InputManager::charCallback(GLFWwindow * window, unsigned int codepoint)
{
	auto inputManager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
	inputManager->onCharacterEnteredEvent(codepoint);
}

void InputManager::mouseMovedCallback(GLFWwindow* window, double x, double y) {
	auto inputManager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
	inputManager->onMouseMovedEvent(x, y);
}

void InputManager::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	auto inputManager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
	inputManager->onMouseButtonEvent(button, action, mods);
}

void InputManager::scrollCallback(GLFWwindow* window, double offsetX, double offsetY) {
	auto inputManager = reinterpret_cast<InputManager*>(glfwGetWindowUserPointer(window));
	inputManager->onScrollEvent(offsetX, offsetY);
}