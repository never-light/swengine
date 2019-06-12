#pragma once

namespace sw::platform::base
{
	enum class KeyboardKey {
		Unknown,
		Backspace,
		Tab,
		Clear,
		Enter,
		Shift,
		Ctrl,
		Alt,
		Pause,
		CapsLock,
		Escape,
		Space,
		PageUp,
		PageDown,
		End,
		Home,
		LeftArrow,
		UpArrow,
		RightArrow,
		DownArrow,
		PrintScreen,
		Insert,
		Delete,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		Multiply,
		Add,
		Separator,
		Subtract,
		Decimal,
		Divide,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		F16,
		F17,
		F18,
		F19,
		F20,
		F21,
		F22,
		F23,
		F24,
		NumLock,
		ScrollLock,
		LeftShift,
		RightShift,
		LeftCtrl,
		RightCtrl,
		LeftMenu,
		RightMenu,
		Plus,
		Comma,
		Minus,
		Period,
		Tilde,
	};

	enum class KeyboardKeyState {
		Unknown, Pressed, Released
	};

	enum class MouseButton {
		Left,
		Right,
		Middle
	};

	enum class MouseButtonState {
		Unknown, Pressed, Released
	};

	struct CursorPosition {
		int x;
		int y;
	};

	struct MouseState {
		CursorPosition position;

		MouseButtonState leftButtonState;
		MouseButtonState rightButtonState;
	};

	class EventsListener {
	public:
		EventsListener() = default;
		virtual ~EventsListener() = default;

		virtual void onKeyPress(base::KeyboardKey key) {}
		virtual void onKeyRelease(base::KeyboardKey key) {}
		
		virtual void onMouseButtonPress(MouseButton button) {}
		virtual void onMouseButtonRelease(MouseButton button) {}
		virtual void onMouseMove(const CursorPosition& state) {}
	};
}