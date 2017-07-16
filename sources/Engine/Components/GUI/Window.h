#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

class Window {
public:
	Window(const std::string&, int, int);
	~Window();

	GLFWwindow* getWindowPointer() const;
	bool shouldClose() const;

	void close();
private:
	GLFWwindow* m_WindowPointer;
};