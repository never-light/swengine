#include "Window.h"
#include <iostream>
#include <Engine\Components\Debugging\Log.h>

Window::Window(const std::string& title, int width, int height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwInit();

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	this->m_WindowPointer = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!this->m_WindowPointer) {
		errlog() << "Failed to create GLFW window";
		glfwTerminate();
	}

	glfwMakeContextCurrent(this->m_WindowPointer);

	glfwGetFramebufferSize(this->m_WindowPointer, &width, &height);
	glViewport(0, 0, width, height);
}

Window::~Window() {

}

GLFWwindow* Window::getWindowPointer() const {
	return m_WindowPointer;
}

bool Window::shouldClose() const {
	return static_cast<bool>(glfwWindowShouldClose(this->m_WindowPointer));
}

void Window::close() {
	glfwSetWindowShouldClose(this->m_WindowPointer, GL_TRUE);
}