#include "Window.h"
#include <iostream>

#include <Engine/Exceptions/EngineException.h>
#include <Engine/assertions.h>

Window::Window(const std::string& name, int width, int height, bool fullscreen, int samples) 
	: m_width(width),
	m_height(height),
	m_defaultCursor(new Cursor(Cursor::System::Arrow))
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	if (samples > 0)
		glfwWindowHint(GLFW_SAMPLES, samples);

	GLFWmonitor* monitor = (fullscreen) ? glfwGetPrimaryMonitor() : nullptr;
	this->m_windowPointer = glfwCreateWindow(width, height, name.c_str(), monitor, nullptr);

	if (!this->m_windowPointer)
		throw EngineException("Failed to create window", __FILE__, __LINE__, __FUNCTION__);

	glfwMakeContextCurrent(this->m_windowPointer);

	glfwGetFramebufferSize(this->m_windowPointer, &width, &height);
}

Window::~Window() {
	delete m_defaultCursor;
}

void Window::update() {
	if (m_cursorMode == CursorMode::UnlimitedHidden) {
		resetCursorPosition();
	}
}

int16 Window::getWidth() const {
	return m_width;
}

int16 Window::getHeight() const {
	return m_height;
}

void Window::setCursorMode(CursorMode mode) {
	m_cursorMode = mode;

	if (mode == CursorMode::Default)
		glfwSetInputMode(m_windowPointer, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else if (mode == CursorMode::Hidden)
		glfwSetInputMode(m_windowPointer, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	else if (mode == CursorMode::UnlimitedHidden) {
		glfwSetInputMode(m_windowPointer, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		resetCursorPosition();
	}
}

CursorMode Window::getCursorMode() const {
	return m_cursorMode;
}

void Window::resetCurrentCursor()
{
	setCurrentCursor(m_defaultCursor);
}

void Window::setCurrentCursor(Cursor * cursor) {
	_assert(cursor != nullptr);

	m_currentCursor = cursor;
	glfwSetCursor(m_windowPointer, cursor->getRawCursorPointer());
}

Cursor * Window::getCurrentCursor() const
{
	return m_currentCursor;
}

Cursor * Window::getDefaultCursor() const {
	return m_defaultCursor;
}

void Window::setCursorPosition(real64 x, real64 y) {
	glfwSetCursorPos(m_windowPointer, x, y);
}

void Window::resetCursorPosition() {
	setCursorPosition(0, 0);
}

WindowCursorState Window::getCursorPosition() const {
	WindowCursorState state;

	glfwGetCursorPos(m_windowPointer, &state.x, &state.y);

	return state;
}

void Window::getCursorPosition(real64* x, real64* y) const {
	glfwGetCursorPos(m_windowPointer, x, y);
}

void Window::close() {
	glfwSetWindowShouldClose(this->m_windowPointer, GL_TRUE);
}

bool Window::shouldClose() const {
	return static_cast<bool>(glfwWindowShouldClose(this->m_windowPointer));
}

GLFWwindow* Window::getWindowPointer() const {
	return m_windowPointer;
}