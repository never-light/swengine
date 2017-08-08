#pragma once

#include <Engine\types.h>

enum class KeyState {
	Unknown, Pressed, Released, Repeated
};

struct KeyEvent {
	enum class Type {
		Press, Release, Repeat
	};

	enum class Modifier {
		None, Control, Shift, Alt
	};

	int16 key;
	Type type;
	Modifier mod;
};

enum class MouseButtonState {
	Unknown, Pressed, Released
};

struct MousePosition {
	real64 x;
	real64 y;
};

struct MouseState {
	MousePosition position;

	MouseButtonState leftButtonState;
	MouseButtonState rightButtonState;
};