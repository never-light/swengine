#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <utility>

struct SaveCommandTriggerEvent {
 public:
  explicit SaveCommandTriggerEvent(std::string saveName)
    : m_saveName(std::move(saveName))
  {

  }

  [[nodiscard]] const std::string& getSaveName() const
  {
    return m_saveName;
  }

 private:
  std::string m_saveName;
};

class GameObjectSaveWrapper {
 public:
  explicit GameObjectSaveWrapper(GameObject gameObject);

  template<class Archive>
  void save(Archive& archive) const;

  template<class Archive>
  void load(Archive& archive);

 private:
  GameObject m_gameObject;
};

class SavingSystem : public GameSystem,
                     public EventsListener<SaveCommandTriggerEvent> {
 public:
  SavingSystem();
  ~SavingSystem() override;

  void configure() override;
  void unconfigure() override;

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(const SaveCommandTriggerEvent& event) override;

 private:
  void saveGameState(const std::string& saveName);
};
