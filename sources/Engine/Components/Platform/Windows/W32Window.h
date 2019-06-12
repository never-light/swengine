#pragma once

#include <string>
#include <Windows.h>

#include "../Base/BaseWindow.h"
#include "W32Cursor.h"

namespace sw::platform::windows
{
class Window : public base::Window {
public:
	Window(const std::string& title, int width, int height);
	virtual ~Window();

	MessagesProcessingResult processNewMessages() override;
	void close() override;

	int getClientWidth() const override;
	int getClientHeight() const override;

	int getFullWidth() const override;
	int getFullHeight() const override;

	void show() override;
	void hide() override;

	void setCursorBehaviour(CursorBehaviour behaviour) override;
	CursorBehaviour getCursorBehaviour() const override;

	void setCursor(std::shared_ptr<base::Cursor> cursor) override;
	Cursor* getCursor() const override;

	void setCursorPosition(const base::CursorPosition& position) override;
	base::CursorPosition getCursorPosition() const override;

	void addEventsListener(std::shared_ptr<base::EventsListener> listener) override;
	void removeEventsListener(base::EventsListener* listener) override;

	base::KeyboardKeyState getKeyboardKeyState(base::KeyboardKey key) const override;
	base::MouseButtonState getMouseButtonState(base::MouseButton button) const override;
	base::MouseState getMouseState() const override;

	HWND getNativeHandle() const;
	const char* getNativeClassName() const;

private:
	void onKeyPress(base::KeyboardKey key);
	void onKeyRelease(base::KeyboardKey key);

	void onMouseMove(const base::CursorPosition& position);
	void onMouseButtonPress(const base::MouseButton& button);
	void onMouseButtonRelease(const base::MouseButton& button);

private:
	ATOM registerClass(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	static void initializeWindowsPlatform();

private:
	std::string m_title;

	int m_width = 0;
	int m_height = 0;

	std::shared_ptr<windows::Cursor> m_cursor = nullptr;
	CursorBehaviour m_cursorBehaviour = CursorBehaviour::Default;

	std::vector<std::shared_ptr<base::EventsListener>> m_eventsListeners;
	base::KeyboardKeyState m_keyboardKeysState[512];
	base::MouseState m_mouseState;

	HWND m_handle = nullptr;

	const char* m_windowClassName = "SWEngine";
private:
	static base::KeyboardKey s_virtualKeysLookupTable[256];
};
}