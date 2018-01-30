#pragma once

#include <Engine\types.h>
#include <Engine\Components\Graphics\OpenGL3\OpenGL3.h>

#include <string>

#include "Viewport.h"

struct WindowCursorState {
	real64 x;
	real64 y;
};

enum class CursorType {
	Default, Hidden, Manual
};

class Window {
public:

public:
	Window(const std::string& name, int width, int height);
	~Window();

	void update();
	void render();

	int16 getWidth() const;
	int16 getHeight() const;

	void setCursorType(CursorType);
	CursorType getCursorType() const;
	
	Viewport* getViewport() const;
	void setViewport(Viewport* viewport);

	void setCursorPosition(real64, real64);
	void resetCursorPosition();
	WindowCursorState getCursorPosition() const;
	void getCursorPosition(real64*, real64*) const;

	void close();
	bool shouldClose() const;

	GLFWwindow* getWindowPointer() const;
private:
	Viewport* m_viewport;

	GLFWwindow* m_windowPointer;
	CursorType m_cursorType;

	int16 m_width;
	int16 m_height;
};