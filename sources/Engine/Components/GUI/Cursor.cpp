#include "Cursor.h"

Cursor::Cursor(Cursor::System systemCursor) {
	int glfwCursorShape;

	if (systemCursor == Cursor::System::Arrow)
		glfwCursorShape = GLFW_ARROW_CURSOR;

	m_cursorPointer = glfwCreateStandardCursor(glfwCursorShape);
}

Cursor::Cursor(const RawImage & image) {
	GLFWimage cursorImage;
	cursorImage.width = image.getWidth();
	cursorImage.height = image.getHeight();
	cursorImage.pixels = (unsigned char*)image.getRawData();
	
	m_cursorPointer = glfwCreateCursor(&cursorImage, 0, 0);
}

Cursor::~Cursor() {
	glfwDestroyCursor(m_cursorPointer);
}

GLFWcursor * Cursor::getRawCursorPointer() const
{
	return m_cursorPointer;
}
