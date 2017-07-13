#include "OpenGL3Window.h"
#include <iostream>

OpenGL3Window::OpenGL3Window(const std::string& title, int width, int height) {
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	this->m_WindowPointer = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!this->m_WindowPointer) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwMakeContextCurrent(this->m_WindowPointer);

	glfwGetFramebufferSize(this->m_WindowPointer, &width, &height);
	glViewport(0, 0, width, height);
}

OpenGL3Window::~OpenGL3Window() {

}

GLFWwindow* OpenGL3Window::getWindowPointer() const {
	return m_WindowPointer;
}

bool OpenGL3Window::shouldClose() const {
	return static_cast<bool>(glfwWindowShouldClose(this->m_WindowPointer));
}

void OpenGL3Window::close() {
	glfwSetWindowShouldClose(this->m_WindowPointer, GL_TRUE);
}