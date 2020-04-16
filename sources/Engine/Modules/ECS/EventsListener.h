#pragma once

#include "BaseEventsListener.h"

class GameWorld;

enum class EventProcessStatus {
  Processed,
  Prevented
};

/*!
 * \brief Class for representing an event listener 
 */
template<class T>
class EventsListener : public BaseEventsListener {
 public:
  ~EventsListener() override = default;

  /*!
   * \brief Receives an event data
   *
   * \param gameWorld game world pointer
   * \param event event data
   *
   * \return event process status
   */
  virtual EventProcessStatus receiveEvent(GameWorld* gameWorld, const T& event) = 0;
};
