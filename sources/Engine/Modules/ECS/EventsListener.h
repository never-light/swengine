#pragma once

#include "BaseEventsListener.h"

class GameWorld;

/*!
 * \brief Class for representing an event listener 
 */
template<class T>
class EventsListener : public BaseEventsListener {
public:
	virtual ~EventsListener() = default;

	/*!
	 * \brief Receives an event data
	 * 
	 * \param gameWorld game world pointer
	 * \param event event data
	 */
	virtual void receiveEvent(GameWorld* gameWorld, const T& event) = 0;
};