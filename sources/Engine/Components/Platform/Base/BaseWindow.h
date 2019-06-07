#pragma once

#include <string>
#include <functional>
#include <memory>

#include <Engine/Components/Math/Math.h>

#include "Input.h"
#include "BaseCursor.h"

namespace sw::platform::base {
class Window {
public:
	enum class CursorBehaviour {
		Default, Hidden
	};

	enum class MessagesProcessingResult {
		Processed, FoundExitMessage
	};

public:
	virtual ~Window() = default;
	
	virtual MessagesProcessingResult processNewMessages() = 0;
	virtual void close() = 0;

	virtual int getWidth() const = 0;
	virtual int getHeight() const = 0;

	virtual void show() = 0;
	virtual void hide() = 0;

	virtual void setCursorBehaviour(CursorBehaviour behaviour) = 0;
	virtual CursorBehaviour getCursorBehaviour() const = 0;

	virtual void setCursor(std::shared_ptr<Cursor> cursor) = 0;
	virtual Cursor* getCursor() const = 0;

	virtual void setCursorPosition(const CursorPosition& position) = 0;		
	virtual CursorPosition getCursorPosition() const = 0;
		
	virtual void addEventsListener(std::shared_ptr<EventsListener> listener) = 0;
	virtual void removeEventsListener(EventsListener* listener) = 0;

	virtual KeyboardKeyState getKeyboardKeyState(KeyboardKey key) const = 0;
	virtual MouseButtonState getMouseButtonState(MouseButton button) const = 0;
	virtual MouseState getMouseState() const = 0;
};
}
