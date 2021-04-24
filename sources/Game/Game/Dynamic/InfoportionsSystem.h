#pragma once

#include <string>
#include <unordered_set>

#include <Engine/Modules/ECS/ECS.h>

struct ToggleInfoportionEvent {
 public:
  ToggleInfoportionEvent(GameObject actor, std::string infoportionName);

  [[nodiscard]] GameObject getActor() const;
  [[nodiscard]] const std::string& getInfoportionName() const;

 private:
  GameObject m_actor;
  std::string m_infoportionName;

 public:
  static const auto IS_LOGGING_ALLOWED = true;

};

struct AddInfoportionCommandEvent : public ToggleInfoportionEvent {
 public:
  AddInfoportionCommandEvent(GameObject actor, std::string infoportionName);

 public:
  static const auto IS_LOGGING_ALLOWED = true;

  inline std::string logData() const
  {
    return fmt::format("actor {}, info_name {}",
      getActor().getName(), getInfoportionName());
  }

};

struct RemoveInfoportionCommandEvent : public ToggleInfoportionEvent {
 public:
  RemoveInfoportionCommandEvent(GameObject actor, std::string infoportionName);

 public:
  static const auto IS_LOGGING_ALLOWED = true;

  inline std::string logData() const
  {
    return fmt::format("actor {}, info_name {}",
      getActor().getName(), getInfoportionName());
  }
};

struct RemoveInfoportionEvent : public ToggleInfoportionEvent {
 public:
  RemoveInfoportionEvent(GameObject actor, std::string infoportionName);

 public:
  static const auto IS_LOGGING_ALLOWED = true;

  inline std::string logData() const
  {
    return fmt::format("actor {}, info_name {}",
      getActor().getName(), getInfoportionName());
  }
};

struct AddInfoportionEvent : public ToggleInfoportionEvent {
 public:
  AddInfoportionEvent(GameObject actor, std::string infoportionName);

 public:
  static const auto IS_LOGGING_ALLOWED = true;

  inline std::string logData() const
  {
    return fmt::format("actor {}, info_name {}",
      getActor().getName(), getInfoportionName());
  }
};

class ActorInfoportionsStorage {
 public:
  ActorInfoportionsStorage() = default;
  ~ActorInfoportionsStorage() = default;

  void addInfoportion(const std::string& infoportion);
  void removeInfoportion(const std::string& infoportion);

  [[nodiscard]] bool hasInfoportion(const std::string& infoportion) const;

 private:
  std::unordered_set<std::string> m_storage;
};

class InfoportionsSystem : public GameSystem,
                           public EventsListener<AddInfoportionCommandEvent>,
                           public EventsListener<RemoveInfoportionCommandEvent> {
 public:
  InfoportionsSystem() = default;

  void addInfoportion(const std::string& infoportion);
  [[nodiscard]] bool hasInfoportion(const std::string& infoportion) const;

  void loadInfoportionsFromFile(const std::string& path);

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(const AddInfoportionCommandEvent& event) override;
  EventProcessStatus receiveEvent(const RemoveInfoportionCommandEvent& event) override;

 private:
  std::unordered_set<std::string> m_storage;
};
