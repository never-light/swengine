#pragma once

#include "BaseEventsListener.h"

class GameWorld;

enum class EventProcessStatus {
  Processed,
  Prevented,
  Skipped
};

struct EventsTypeInfo {
  template<class T>
  static size_t getTypeIndex()
  {
    static size_t typeIndex = s_typeIndex++;

    return typeIndex;
  }

  static size_t s_typeIndex;
};

/*!
 * \brief Class for representing an event listener 
 */
template<class T>
class EventsListener : virtual public BaseEventsListener {
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
  virtual EventProcessStatus receiveEvent(const T& event) = 0;
};
