#pragma once

#include "BaseEventsListener.h"

class GameWorld;

template<class T>
class EventsListener : public BaseEventsListener {
public:
	virtual ~EventsListener() = default;

	virtual void receiveEvent(GameWorld* gameWorld, const T& event) = 0;
};