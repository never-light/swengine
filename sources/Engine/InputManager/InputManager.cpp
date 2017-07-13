#include "InputManager.h"

InputManager::InputManager(Window* window) : m_window(window) {

}

InputManager::~InputManager() {

}

bool InputManager::isKeyPressed(int key) {
	return glfwGetKey(m_window->getWindowPointer(), key) == GLFW_PRESS;
}