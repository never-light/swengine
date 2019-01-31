#pragma once

#include "RawImage.h"
#include <Engine/Components/Graphics/OpenGL4/GL.h>

class Cursor {
public:
	enum class System {
		Arrow
	};

public:
	Cursor(Cursor::System systemCursor);
	Cursor(const RawImage& image);
	~Cursor();

	GLFWcursor* getRawCursorPointer() const;
private:
	GLFWcursor * m_cursorPointer;
};