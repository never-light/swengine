#pragma once

#include <string>
#include <vector>

#include <Engine/Modules/ECS/ECS.h>
#include "InfoportionsSystem.h"

/**
 * @brief Command to initiate dialogue between player (initiator)
 * and specified NPC (target).
 *
 * This command intended to be handled in player controlling system.
 */
struct ActorTalkTriggerCommandEvent {
 public:
  ActorTalkTriggerCommandEvent(GameObject initiator,
    GameObject target)
    : initiator(initiator),
      target(target)
  {

  }

 public:
  GameObject initiator;
  GameObject target;
};

enum class QuestTaskState {
  NotStarted,
  Started,
  Completed,
  Failed
};

enum class QuestState {
  NotStarted,
  Started,
  Completed,
  Failed
};

struct ActorQuestTaskState {
 public:
  explicit ActorQuestTaskState(std::string taskId);

  [[nodiscard]] const std::string& getTaskId() const;

  void setState(QuestTaskState state);
  [[nodiscard]] QuestTaskState getState() const;

 private:
  std::string m_taskId;
  QuestTaskState m_taskState = QuestTaskState::NotStarted;
};

struct ActorQuestState {
 public:
  explicit ActorQuestState(std::string questId);

  [[nodiscard]] const std::string& getQuestId() const;

  void addTaskState(const std::string& taskId);
  [[nodiscard]] ActorQuestTaskState& getTaskState(const std::string& taskId);

  void setState(QuestState state);
  [[nodiscard]] QuestState getState() const;

  void setCurrentTaskId(const std::string& taskId);
  [[nodiscard]] const std::string& getCurrentTaskId() const;

 private:
  std::string m_questId;
  std::unordered_map<std::string, ActorQuestTaskState> m_tasksStates;
  QuestState m_questState = QuestState::NotStarted;

  std::string m_currentTaskId;
};

struct ActorDialogue {
 public:
  ActorDialogue(std::string dialogueId, bool isStartedByNCP);

  [[nodiscard]] const std::string& getDialogueId() const;
  [[nodiscard]] bool isStartedByNPC() const;

 private:
  std::string m_dialogueId;
  bool m_isStartedByNCP{};
};

struct ActorComponent {
 public:
  ActorComponent() = default;

  void setName(const std::string& name);
  [[nodiscard]] const std::string& getName() const;

  void addDialogue(const ActorDialogue& dialogue);
  [[nodiscard]] const std::vector<ActorDialogue>& getDialogues() const;

  [[nodiscard]] bool hasDialogue(const std::string& dialogueId) const;

  [[nodiscard]] ActorInfoportionsStorage& getInfoportionsStorage();
  [[nodiscard]] const ActorInfoportionsStorage& getInfoportionsStorage() const;

  void addQuestState(const std::string& questId);
  [[nodiscard]] ActorQuestState& getQuestState(const std::string& questId);

 private:
  std::string m_name;
  std::vector<ActorDialogue> m_dialogues;
  ActorInfoportionsStorage m_infoportionsStorage;

  std::unordered_map<std::string, ActorQuestState> m_questsStates;
};
