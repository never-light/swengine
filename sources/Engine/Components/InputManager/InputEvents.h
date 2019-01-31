#pragma once

#include <Engine/types.h>

using Key = uint16;
using MouseButton = uint16;

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

	Key key;
	Type type;
	Modifier mod;
};

enum class MouseButtonState {
	Unknown, Pressed, Released
};

struct MousePosition {
	double x;
	double y;
};

struct MouseState {
	MousePosition position;

	MouseButtonState leftButtonState;
	MouseButtonState rightButtonState;
};