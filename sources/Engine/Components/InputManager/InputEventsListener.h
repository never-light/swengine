#pragma once

#include <Engine\types.h>
#include "InputEvents.h"

class InputEventsListener {
public:
	InputEventsListener();
	virtual ~InputEventsListener();

	virtual void onKeyPress(Key key, KeyEvent::Modifier mod);
	virtual void onKeyRelease(Key key, KeyEvent::Modifier mod);
	virtual void onKeyRepeat(Key key, KeyEvent::Modifier mod);

	virtual void onCharacterEntered(unsigned char character);

	virtual void onMouseButtonPress(MouseButton button, const MouseState&);
	virtual void onMouseButtonRelease(MouseButton button, const MouseState&);
	virtual void onMouseMove(const MouseState&);

	virtual void onScroll(double offsetX, double offsetY);
};