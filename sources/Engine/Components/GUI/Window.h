#pragma once

#include <Engine/types.h>
#include <Engine/Components/Graphics/OpenGL4/GL.h>

#include "Cursor.h"

#include <string>

struct WindowCursorState {
	real64 x;
	real64 y;
};

enum class CursorMode {
	Default, Hidden, UnlimitedHidden
};

class Window {
public:

public:
	Window(const std::string& name, int width, int height, bool fullscreen = false, int samples = 0);
	~Window();

	void update();

	int16 getWidth() const;
	int16 getHeight() const;

	void setCursorMode(CursorMode mode);
	CursorMode getCursorMode() const;

	void resetCurrentCursor();
	void setCurrentCursor(Cursor* cursor);
	Cursor* getCurrentCursor() const;

	Cursor* getDefaultCursor() const;

	void setCursorPosition(real64, real64);
	void resetCursorPosition();
	WindowCursorState getCursorPosition() const;
	void getCursorPosition(real64*, real64*) const;

	void close();
	bool shouldClose() const;

	GLFWwindow* getWindowPointer() const;

private:
	GLFWwindow* m_windowPointer;
	CursorMode m_cursorMode;

	int16 m_width;
	int16 m_height;

	Cursor* m_currentCursor;
	Cursor* m_defaultCursor;
};