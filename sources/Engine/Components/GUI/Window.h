#pragma once

#include <Engine\Components\Graphics\OpenGL3\OpenGL3.h>

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