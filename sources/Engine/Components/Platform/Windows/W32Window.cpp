#include "W32Window.h"
#include "Engine/Exceptions/EngineException.h"
#include <winuser.h>
#include <windowsx.h>

#include <algorithm>

namespace sw::platform::windows
{
base::KeyboardKey Window::s_virtualKeysLookupTable[256] = {};

Window::Window(const std::string& title, int width, int height)
	: m_title(title),
	m_width(width),
	m_height(height),
	m_cursorBehaviour(CursorBehaviour::Default),
	m_handle(nullptr),
	m_mouseState({{0, 0}, base::MouseButtonState::Unknown, base::MouseButtonState::Unknown})
{
	assert(width > 0 && height > 0);

	initializeWindowsPlatform();

	HINSTANCE hInstance = GetModuleHandle(nullptr);
	LPCSTR windowClass = MAKEINTATOM(registerClass(hInstance));

	if (windowClass == 0) {
		THROW_ENGINE_ERROR("Failed to register the window class");
	}

	HWND handle = CreateWindow(
		windowClass, title.c_str(),
		WS_CAPTION | WS_SYSMENU | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		CW_USEDEFAULT, 0, m_width, m_height,
		NULL, NULL, hInstance, this
	);

	if (handle == nullptr) {
		THROW_ENGINE_ERROR("Failed to create the window");
	}

	m_handle = handle;

	POINT p;

	if (!GetCursorPos(&p))
		THROW_ENGINE_ERROR("Failed to get initial cursor position");

	m_mouseState.position = {p.x, p.y};
}

Window::~Window()
{
	if (m_handle != nullptr) {
		DestroyWindow(m_handle);
	}
}

Window::MessagesProcessingResult Window::processNewMessages()
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT)
		return MessagesProcessingResult::FoundExitMessage;

	return MessagesProcessingResult::Processed;
}

void Window::close()
{
	PostQuitMessage(0);
}

int Window::getWidth() const
{
	return m_width;
}

int Window::getHeight() const
{
	return m_height;
}

void Window::show()
{
	ShowWindow(m_handle, SW_SHOW);
}

void Window::hide()
{
	ShowWindow(m_handle, SW_HIDE);
}

void Window::setCursorBehaviour(CursorBehaviour behaviour)
{
	m_cursorBehaviour = behaviour;
}

Window::CursorBehaviour Window::getCursorBehaviour() const
{
	return m_cursorBehaviour;
}

void Window::setCursor(std::shared_ptr<base::Cursor> cursor)
{
	assert(cursor != nullptr);

	m_cursor = std::dynamic_pointer_cast<Cursor>(cursor);

	if (m_cursor == nullptr)
		THROW_ENGINE_ERROR("The cursor has invalid type");

	SetCursor(m_cursor->getNativeHandle());
}

Cursor* Window::getCursor() const
{
	return m_cursor.get();
}

void Window::setCursorPosition(const base::CursorPosition& position)
{
	m_mouseState.position = position;

	POINT internalPosition = { position.x, position.y };
	ClientToScreen(m_handle, &internalPosition);
	SetCursorPos(internalPosition.x, internalPosition.y);
}

base::CursorPosition Window::getCursorPosition() const
{
	POINT position;

	if (!GetCursorPos(&position))
		THROW_ENGINE_ERROR("Failed to get the cursor position");

	ScreenToClient(m_handle, &position);

	return { position.x, position.y };
}

void Window::addEventsListener(std::shared_ptr<base::EventsListener> listener)
{
	assert(listener != nullptr);

	m_eventsListeners.push_back(listener);
}

void Window::removeEventsListener(base::EventsListener* listener)
{
	auto it = m_eventsListeners.begin();

	while (it != m_eventsListeners.end() && (*it).get() != listener)
		++it;

	if (it != m_eventsListeners.end())
		m_eventsListeners.erase(it);
}

base::KeyboardKeyState Window::getKeyboardKeyState(base::KeyboardKey key) const
{
	return m_keyboardKeysState[static_cast<int>(key)];
}

base::MouseButtonState Window::getMouseButtonState(base::MouseButton button) const
{
	if (button == base::MouseButton::Left)
		return m_mouseState.leftButtonState;

	if (button == base::MouseButton::Right)
		return m_mouseState.rightButtonState;

	THROW_ENGINE_ERROR("Wrong mouse button");
}

base::MouseState Window::getMouseState() const
{
	return m_mouseState;
}

HWND Window::getNativeHandle() const
{
	return m_handle;
}

const char* Window::getNativeClassName() const
{
	return m_windowClassName;
}

void Window::onKeyPress(base::KeyboardKey key)
{
	m_keyboardKeysState[static_cast<int>(key)] = base::KeyboardKeyState::Pressed;

	for (const auto& listener : m_eventsListeners) {
		listener->onKeyPress(key);
	}
}

void Window::onKeyRelease(base::KeyboardKey key)
{
	m_keyboardKeysState[static_cast<int>(key)] = base::KeyboardKeyState::Released;

	for (const auto& listener : m_eventsListeners) {
		listener->onKeyRelease(key);
	}
} 

void Window::onMouseMove(const base::CursorPosition& position)
{
	m_mouseState.position = position;

	for (const auto& listener : m_eventsListeners) {
		listener->onMouseMove(position);
	}
}

void Window::onMouseButtonPress(const base::MouseButton& button)
{
	if (button == base::MouseButton::Left)
		m_mouseState.leftButtonState = base::MouseButtonState::Pressed;

	if (button == base::MouseButton::Right)
		m_mouseState.rightButtonState = base::MouseButtonState::Pressed;

	for (const auto& listener : m_eventsListeners) {
		listener->onMouseButtonPress(button);
	}
}

void Window::onMouseButtonRelease(const base::MouseButton& button)
{
	if (button == base::MouseButton::Left)
		m_mouseState.leftButtonState = base::MouseButtonState::Released;

	if (button == base::MouseButton::Right)
		m_mouseState.rightButtonState = base::MouseButtonState::Released;

	for (const auto& listener : m_eventsListeners) {
		listener->onMouseButtonRelease(button);
	}
}

void Window::initializeWindowsPlatform()
{
	s_virtualKeysLookupTable[0x08] = base::KeyboardKey::Backspace;
	s_virtualKeysLookupTable[0x09] = base::KeyboardKey::Tab;
	s_virtualKeysLookupTable[0x0D] = base::KeyboardKey::Enter;
	s_virtualKeysLookupTable[0x10] = base::KeyboardKey::Shift;
	s_virtualKeysLookupTable[0x11] = base::KeyboardKey::Ctrl;
	s_virtualKeysLookupTable[0x12] = base::KeyboardKey::Alt;
	s_virtualKeysLookupTable[0x13] = base::KeyboardKey::Pause;
	s_virtualKeysLookupTable[0x14] = base::KeyboardKey::CapsLock;
	s_virtualKeysLookupTable[0x1B] = base::KeyboardKey::Escape;
	s_virtualKeysLookupTable[0x20] = base::KeyboardKey::Space;
	s_virtualKeysLookupTable[0x21] = base::KeyboardKey::PageUp;
	s_virtualKeysLookupTable[0x22] = base::KeyboardKey::PageDown;
	s_virtualKeysLookupTable[0x23] = base::KeyboardKey::End;
	s_virtualKeysLookupTable[0x24] = base::KeyboardKey::Home;
	s_virtualKeysLookupTable[0x25] = base::KeyboardKey::LeftArrow;
	s_virtualKeysLookupTable[0x26] = base::KeyboardKey::UpArrow;
	s_virtualKeysLookupTable[0x27] = base::KeyboardKey::RightArrow;
	s_virtualKeysLookupTable[0x28] = base::KeyboardKey::DownArrow;
	s_virtualKeysLookupTable[0x2C] = base::KeyboardKey::PrintScreen;
	s_virtualKeysLookupTable[0x2D] = base::KeyboardKey::Insert;
	s_virtualKeysLookupTable[0x2E] = base::KeyboardKey::Delete;
	s_virtualKeysLookupTable[0x30] = base::KeyboardKey::Num0;
	s_virtualKeysLookupTable[0x31] = base::KeyboardKey::Num1;
	s_virtualKeysLookupTable[0x32] = base::KeyboardKey::Num2;
	s_virtualKeysLookupTable[0x33] = base::KeyboardKey::Num3;
	s_virtualKeysLookupTable[0x34] = base::KeyboardKey::Num4;
	s_virtualKeysLookupTable[0x35] = base::KeyboardKey::Num5;
	s_virtualKeysLookupTable[0x36] = base::KeyboardKey::Num6;
	s_virtualKeysLookupTable[0x37] = base::KeyboardKey::Num7;
	s_virtualKeysLookupTable[0x38] = base::KeyboardKey::Num8;
	s_virtualKeysLookupTable[0x39] = base::KeyboardKey::Num9;
	s_virtualKeysLookupTable[0x41] = base::KeyboardKey::A;
	s_virtualKeysLookupTable[0x42] = base::KeyboardKey::B;
	s_virtualKeysLookupTable[0x43] = base::KeyboardKey::C;
	s_virtualKeysLookupTable[0x44] = base::KeyboardKey::D;
	s_virtualKeysLookupTable[0x45] = base::KeyboardKey::E;
	s_virtualKeysLookupTable[0x46] = base::KeyboardKey::F;
	s_virtualKeysLookupTable[0x47] = base::KeyboardKey::G;
	s_virtualKeysLookupTable[0x48] = base::KeyboardKey::H;
	s_virtualKeysLookupTable[0x49] = base::KeyboardKey::I;
	s_virtualKeysLookupTable[0x4A] = base::KeyboardKey::J;
	s_virtualKeysLookupTable[0x4B] = base::KeyboardKey::K;
	s_virtualKeysLookupTable[0x4C] = base::KeyboardKey::L;
	s_virtualKeysLookupTable[0x4D] = base::KeyboardKey::M;
	s_virtualKeysLookupTable[0x4E] = base::KeyboardKey::N;
	s_virtualKeysLookupTable[0x4F] = base::KeyboardKey::O;
	s_virtualKeysLookupTable[0x50] = base::KeyboardKey::P;
	s_virtualKeysLookupTable[0x51] = base::KeyboardKey::Q;
	s_virtualKeysLookupTable[0x52] = base::KeyboardKey::R;
	s_virtualKeysLookupTable[0x53] = base::KeyboardKey::S;
	s_virtualKeysLookupTable[0x54] = base::KeyboardKey::T;
	s_virtualKeysLookupTable[0x55] = base::KeyboardKey::U;
	s_virtualKeysLookupTable[0x56] = base::KeyboardKey::V;
	s_virtualKeysLookupTable[0x57] = base::KeyboardKey::W;
	s_virtualKeysLookupTable[0x58] = base::KeyboardKey::X;
	s_virtualKeysLookupTable[0x59] = base::KeyboardKey::Y;
	s_virtualKeysLookupTable[0x5A] = base::KeyboardKey::Z;
	s_virtualKeysLookupTable[0x60] = base::KeyboardKey::Numpad0;
	s_virtualKeysLookupTable[0x61] = base::KeyboardKey::Numpad1;
	s_virtualKeysLookupTable[0x62] = base::KeyboardKey::Numpad2;
	s_virtualKeysLookupTable[0x63] = base::KeyboardKey::Numpad3;
	s_virtualKeysLookupTable[0x64] = base::KeyboardKey::Numpad4;
	s_virtualKeysLookupTable[0x65] = base::KeyboardKey::Numpad5;
	s_virtualKeysLookupTable[0x66] = base::KeyboardKey::Numpad6;
	s_virtualKeysLookupTable[0x67] = base::KeyboardKey::Numpad7;
	s_virtualKeysLookupTable[0x68] = base::KeyboardKey::Numpad8;
	s_virtualKeysLookupTable[0x69] = base::KeyboardKey::Numpad9;
	s_virtualKeysLookupTable[0x6A] = base::KeyboardKey::Multiply;
	s_virtualKeysLookupTable[0x6B] = base::KeyboardKey::Add;
	s_virtualKeysLookupTable[0x6C] = base::KeyboardKey::Separator;
	s_virtualKeysLookupTable[0x6D] = base::KeyboardKey::Subtract;
	s_virtualKeysLookupTable[0x6E] = base::KeyboardKey::Decimal;
	s_virtualKeysLookupTable[0x6F] = base::KeyboardKey::Divide;
	s_virtualKeysLookupTable[0x70] = base::KeyboardKey::F1;
	s_virtualKeysLookupTable[0x71] = base::KeyboardKey::F2;
	s_virtualKeysLookupTable[0x72] = base::KeyboardKey::F3;
	s_virtualKeysLookupTable[0x73] = base::KeyboardKey::F4;
	s_virtualKeysLookupTable[0x74] = base::KeyboardKey::F5;
	s_virtualKeysLookupTable[0x75] = base::KeyboardKey::F6;
	s_virtualKeysLookupTable[0x76] = base::KeyboardKey::F7;
	s_virtualKeysLookupTable[0x77] = base::KeyboardKey::F8;
	s_virtualKeysLookupTable[0x78] = base::KeyboardKey::F9;
	s_virtualKeysLookupTable[0x79] = base::KeyboardKey::F10;
	s_virtualKeysLookupTable[0x7A] = base::KeyboardKey::F11;
	s_virtualKeysLookupTable[0x7B] = base::KeyboardKey::F12;
	s_virtualKeysLookupTable[0x7C] = base::KeyboardKey::F13;
	s_virtualKeysLookupTable[0x7D] = base::KeyboardKey::F14;
	s_virtualKeysLookupTable[0x7E] = base::KeyboardKey::F15;
	s_virtualKeysLookupTable[0x7F] = base::KeyboardKey::F16;
	s_virtualKeysLookupTable[0x80] = base::KeyboardKey::F17;
	s_virtualKeysLookupTable[0x81] = base::KeyboardKey::F18;
	s_virtualKeysLookupTable[0x82] = base::KeyboardKey::F19;
	s_virtualKeysLookupTable[0x83] = base::KeyboardKey::F20;
	s_virtualKeysLookupTable[0x84] = base::KeyboardKey::F21;
	s_virtualKeysLookupTable[0x85] = base::KeyboardKey::F22;
	s_virtualKeysLookupTable[0x86] = base::KeyboardKey::F23;
	s_virtualKeysLookupTable[0x87] = base::KeyboardKey::F24;
	s_virtualKeysLookupTable[0x90] = base::KeyboardKey::NumLock;
	s_virtualKeysLookupTable[0x91] = base::KeyboardKey::ScrollLock;
	s_virtualKeysLookupTable[0xA0] = base::KeyboardKey::LeftShift;
	s_virtualKeysLookupTable[0xA1] = base::KeyboardKey::RightShift;
	s_virtualKeysLookupTable[0xA2] = base::KeyboardKey::LeftCtrl;
	s_virtualKeysLookupTable[0xA3] = base::KeyboardKey::RightCtrl;
	s_virtualKeysLookupTable[0xA4] = base::KeyboardKey::LeftMenu;
	s_virtualKeysLookupTable[0xA5] = base::KeyboardKey::RightMenu;
	s_virtualKeysLookupTable[0xBB] = base::KeyboardKey::Plus;
	s_virtualKeysLookupTable[0xBC] = base::KeyboardKey::Comma;
	s_virtualKeysLookupTable[0xBD] = base::KeyboardKey::Minus;
	s_virtualKeysLookupTable[0xBE] = base::KeyboardKey::Period;
}

ATOM Window::registerClass(HINSTANCE hInstance)
{
	assert(m_handle == nullptr);

	m_cursor = std::make_shared<Cursor>();

	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(wcex);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = m_cursor->getNativeHandle();
	wcex.lpszClassName = (LPCSTR)getNativeClassName();

	return RegisterClassEx(&wcex);
}

LRESULT Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* window = nullptr;

	if (message == WM_NCCREATE) {
		window = static_cast<Window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window))) {
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else {
		window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	switch (message) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
		{
			base::KeyboardKey key = s_virtualKeysLookupTable[HIWORD(lParam) & 0x1FF];

			if (message == WM_KEYDOWN || message == WM_SYSKEYDOWN)
				window->onKeyRelease(key);
			else if (message == WM_KEYUP || message == WM_SYSKEYUP)
				window->onKeyPress(key);

			break;
		}

	case WM_MOUSEMOVE:
		{
			base::CursorPosition position{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};

			window->onMouseMove(position);
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
		{
			base::MouseButton button = base::MouseButton::Left;

			if (message == WM_RBUTTONDOWN || message == WM_RBUTTONUP)
				button = base::MouseButton::Right;
			else if (message == WM_MBUTTONDOWN || message == WM_MBUTTONUP)
				button = base::MouseButton::Left;

			if (message == WM_LBUTTONDOWN || message == WM_RBUTTONDOWN || message == WM_MBUTTONDOWN)
				window->onMouseButtonPress(button);
			else
				window->onMouseButtonRelease(button);
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
}
