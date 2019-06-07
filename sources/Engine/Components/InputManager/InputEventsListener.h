#pragma once

#include "Events.h"

class InputEventsListener {
public:
	InputEventsListener();
	virtual ~InputEventsListener();

	virtual void onKeyPress(KeyboardKey key);
	virtual void onKeyRelease(KeyboardKey key);
	virtual void onKeyRepeat(KeyboardKey key);

	virtual void onCharacterEntered(unsigned char character);

	virtual void onMouseButtonPress(MouseButton button);
	virtual void onMouseButtonRelease(MouseButton button);
	virtual void onMouseMove(const CursorPosition& position);

	virtual void onScroll(double offsetX, double offsetY);
};