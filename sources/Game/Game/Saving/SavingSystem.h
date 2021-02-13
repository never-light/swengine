#pragma once

#include <utility>

#include <Engine/Modules/ECS/ECS.h>
#include <Engine/Modules/ResourceManagement/ResourcesManager.h>
#include <Engine/Modules/LevelsManagement/LevelsManager.h>
#include <Engine/Utility/DataArchive.h>
#include "Game/Game.h"

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

struct SaveHeader {
  std::string saveName;
  std::string levelName;

  template<class Archive>
  void serialize(Archive& archive)
  {
    archive(
      cereal::make_nvp("save_name", saveName),
      cereal::make_nvp("level_name", levelName));
  };

};

class SavingSystem : public GameSystem,
                     public EventsListener<GameConsoleCommandEvent>,
                     public EventsListener<SaveCommandTriggerEvent>,
                     public EventsListener<LoadCommandTriggerEvent> {
 public:
  SavingSystem(std::shared_ptr<LevelsManager> levelsManager,
    std::shared_ptr<Game> game);
  ~SavingSystem() override;

  void configure() override;
  void unconfigure() override;

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(const GameConsoleCommandEvent& event) override;
  EventProcessStatus receiveEvent(const SaveCommandTriggerEvent& event) override;
  EventProcessStatus receiveEvent(const LoadCommandTriggerEvent& event) override;

 private:
  void saveGameState(const std::string& saveName);
  void loadGameState(const std::string& saveName);

 private:
  std::shared_ptr<LevelsManager> m_levelsManager;
  std::shared_ptr<Game> m_game;
};
