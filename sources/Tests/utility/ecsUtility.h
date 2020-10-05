#pragma once

#include <functional>
#include <optional>

#include <Engine/Modules/ECS/ECS.h>

template<class EventType>
class GenericEventsListener : public EventsListener<EventType> {
 public:
  explicit GenericEventsListener(const std::function<void(const EventType&)>& action)
    : m_action(action)
  {

  }

  void resetReceivedState()
  {
    m_isReceived = false;
    m_events.clear();
  }

  [[nodiscard]] bool isReceived() const
  {
    return m_isReceived;
  }

  [[nodiscard]] size_t getEventsCount() const
  {
    return m_events.size();
  }

  [[nodiscard]] const EventType& getEvent(size_t eventIndex = 0) const
  {
    return m_events.at(eventIndex).value();
  }

  EventProcessStatus receiveEvent(const EventType& event) override
  {
    m_isReceived = true;
    m_events.push_back(event);

    if (m_action != nullptr) {
      m_action(event);
    }

    return EventProcessStatus::Processed;
  }

 private:
  std::function<void(const EventType&)> m_action{};
  std::vector<std::optional<EventType>> m_events{};

  bool m_isReceived{};
};

class ECSUtility {
 public:

  template<class EventType>
  static std::shared_ptr<GenericEventsListener<EventType>> makeEventsListener(const std::function<void(const EventType&)>& action)
  {
    return std::make_shared<GenericEventsListener<EventType>>(action);
  };

  template<class EventType>
  static std::shared_ptr<GenericEventsListener<EventType>> makeEventsListener()
  {
    return std::make_shared<GenericEventsListener<EventType>>(nullptr);
  };
};
