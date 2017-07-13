#pragma once

#include <GLFW/glfw3.h>
#include <string>

class OpenGL3Window {
public:
	OpenGL3Window(const std::string&, int, int);
	~OpenGL3Window();

	GLFWwindow* getWindowPointer() const;
	bool shouldClose() const;

	void close();
private:
	GLFWwindow* m_WindowPointer;
};

typedef OpenGL3Window Window;