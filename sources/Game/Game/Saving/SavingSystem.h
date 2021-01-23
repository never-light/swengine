#pragma once

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/ResourceManagement/ResourcesManager.h>
#include <Engine/Utility/DataArchive.h>

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

struct LoadCommandTriggerEvent {
 public:
  explicit LoadCommandTriggerEvent(std::string saveName)
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

class SavingSystem : public GameSystem,
                     public EventsListener<SaveCommandTriggerEvent>,
                     public EventsListener<LoadCommandTriggerEvent> {
 public:
  explicit SavingSystem();
  ~SavingSystem() override;

  void configure() override;
  void unconfigure() override;

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(const SaveCommandTriggerEvent& event) override;
  EventProcessStatus receiveEvent(const LoadCommandTriggerEvent& event) override;

 private:
  void saveGameState(const std::string& saveName);
  void loadGameState(const std::string& saveName);

};
