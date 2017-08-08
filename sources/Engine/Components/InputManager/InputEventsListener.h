#pragma once

#include <Engine\types.h>
#include "InputEvents.h"

class InputEventsListener {
public:
	InputEventsListener();
	virtual ~InputEventsListener();

	virtual void onKeyPress(int16 key, KeyEvent::Modifier mod);
	virtual void onKeyRelease(int16 key, KeyEvent::Modifier mod);
	virtual void onKeyRepeat(int16 key, KeyEvent::Modifier mod);

	virtual void onMouseButtonPress(int16 button, const MouseState&);
	virtual void onMouseButtonRelease(int16 button, const MouseState&);
	virtual void onMouseMove(const MouseState&);

	virtual void onScroll(real64 offsetX, real64 offsetY);
};