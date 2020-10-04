#pragma once

#include <unordered_map>
#include <utility>

#include "Game/Inventory/InventoryComponent.h"

#include "GameLogicConditionsManager.h"
#include "ActorComponent.h"
#include "Quest.h"

struct QuestStateChangedEvent {
 public:
  QuestStateChangedEvent(const GameObject& actor, std::string questId)
    : m_actor(actor), m_questId(std::move(questId))
  {

  }

  [[nodiscard]] inline GameObject getActor() const
  {
    return m_actor;
  }

  [[nodiscard]] inline const std::string& getQuestId() const
  {
    return m_questId;
  }

 private:
  GameObject m_actor;
  std::string m_questId;
};

struct QuestStartedEvent : public QuestStateChangedEvent {
 public:
  QuestStartedEvent(const GameObject& actor, std::string questId)
    : QuestStateChangedEvent(actor, std::move(questId))
  {

  }
};

struct QuestCompletedEvent : public QuestStateChangedEvent {
 public:
  QuestCompletedEvent(const GameObject& actor, std::string questId)
    : QuestStateChangedEvent(actor, std::move(questId))
  {

  }
};

struct QuestFailedEvent : public QuestStateChangedEvent {
 public:
  QuestFailedEvent(const GameObject& actor, std::string questId)
    : QuestStateChangedEvent(actor, std::move(questId))
  {

  }
};

struct QuestTaskStateChangedEvent : public QuestStateChangedEvent {
 public:
  QuestTaskStateChangedEvent(const GameObject& actor, std::string questId,
    std::string taskId)
    : QuestStateChangedEvent(actor, std::move(questId)),
      m_taskId(std::move(taskId))
  {

  }

  [[nodiscard]] inline const std::string& getTaskId() const
  {
    return m_taskId;
  }

 private:
  std::string m_taskId;
};

struct QuestTaskStartedEvent : public QuestTaskStateChangedEvent {
 public:
  QuestTaskStartedEvent(const GameObject& actor, std::string questId, std::string taskId)
    : QuestTaskStateChangedEvent(actor, std::move(questId), std::move(taskId))
  {

  }
};

struct QuestTaskCompletedEvent : public QuestTaskStateChangedEvent {
 public:
  QuestTaskCompletedEvent(const GameObject& actor, std::string questId, std::string taskId)
    : QuestTaskStateChangedEvent(actor, std::move(questId), std::move(taskId))
  {

  }
};

struct QuestTaskFailedEvent : public QuestTaskStateChangedEvent {
 public:
  QuestTaskFailedEvent(const GameObject& actor, std::string questId, std::string taskId)
    : QuestTaskStateChangedEvent(actor, std::move(questId), std::move(taskId))
  {

  }
};

struct QuestTaskCancelledEvent : public QuestTaskStateChangedEvent {
 public:
  QuestTaskCancelledEvent(const GameObject& actor, std::string questId, std::string taskId)
    : QuestTaskStateChangedEvent(actor, std::move(questId), std::move(taskId))
  {

  }
};

class QuestsStorage {
 public:
  QuestsStorage() = default;

  void addQuest(const Quest& quest);
  [[nodiscard]] const Quest& getQuest(const std::string& questId) const;

  [[nodiscard]] bool hasQuest(const std::string& questId) const;

  [[nodiscard]] const std::unordered_map<std::string, Quest>& getQuests() const;
  [[nodiscard]] std::unordered_map<std::string, Quest>& getQuests();

 private:

  std::unordered_map<std::string, Quest> m_quests;

};

class QuestsSystem : public GameSystem,
                     public EventsListener<AddInfoportionEvent>,
                     public EventsListener<RemoveInfoportionEvent>,
                     public EventsListener<InventoryItemActionEvent>,
                     public EventsListener<GameObjectAddComponentEvent<ActorComponent>> {
 public:
  QuestsSystem(
    std::shared_ptr<GameLogicConditionsManager> conditionsManager,
    std::shared_ptr<QuestsStorage> questsStorage);

  void registerQuest(const Quest& quest);
  void loadQuestsFromFile(const std::string& path);

  void activate() override;
  void deactivate() override;

  EventProcessStatus receiveEvent(const GameObjectAddComponentEvent<ActorComponent>& event) override;
  EventProcessStatus receiveEvent(const AddInfoportionEvent& event) override;
  EventProcessStatus receiveEvent(const RemoveInfoportionEvent& event) override;
  EventProcessStatus receiveEvent(const InventoryItemActionEvent& event) override;

  [[nodiscard]] std::shared_ptr<GameLogicConditionsManager> getConditionsManager() const;

  [[nodiscard]] std::shared_ptr<QuestsStorage> getQuestsStorage() const;

 private:
  void setupActorQuestsState();
  void setupActorQuestState(const Quest& quest);

  void setupConditionsActor(GameLogicCondition* condition);

  GameObject getActor();
  void updateQuestsStates();

  void updateNotStartedQuest(const Quest& quest, ActorQuestState& actorQuestState);
  void updateStartedQuest(const Quest& quest, ActorQuestState& actorQuestState);
  [[nodiscard]] bool activateNewQuestTask(const Quest& quest, ActorQuestState& actorQuestState);

 private:
  std::shared_ptr<GameLogicCondition> loadCondition(const pugi::xml_node& conditionNode);

 private:
  std::shared_ptr<GameLogicConditionsManager> m_conditionsManager;
  std::shared_ptr<QuestsStorage> m_questsStorage;

 private:
  static constexpr auto PLAYER_OBJECT_NAME = "player";
};
