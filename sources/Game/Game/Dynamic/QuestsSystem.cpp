#include "QuestsSystem.h"

#include <utility>

#include <Engine/Utility/files.h>
#include <Engine/Utility/xml.h>
#include <Engine/Exceptions/exceptions.h>

#include "ActorComponent.h"
#include "Quest.h"

void QuestsStorage::addQuest(const Quest& quest)
{
  m_quests.insert({quest.getId(), quest});
}

const Quest& QuestsStorage::getQuest(const std::string& questId) const
{
  return m_quests.at(questId);
}

bool QuestsStorage::hasQuest(const std::string& questId) const
{
  return m_quests.contains(questId);
}

const std::unordered_map<std::string, Quest>& QuestsStorage::getQuests() const
{
  return m_quests;
}

std::unordered_map<std::string, Quest>& QuestsStorage::getQuests()
{
  return m_quests;
}

QuestsSystem::QuestsSystem(
  std::shared_ptr<GameLogicConditionsManager> conditionsManager,
  std::shared_ptr<QuestsStorage> questsStorage)
  : m_conditionsManager(std::move(conditionsManager)),
    m_questsStorage(questsStorage)
{

}

void QuestsSystem::loadQuestsFromFile(const std::string& path)
{
  std::string questsDescriptionPath = FileUtils::getGameResourcePath(
    "quests/" + path + ".xml");

  spdlog::debug("Load quests list from {}", questsDescriptionPath);

  auto questsDescription = XMLUtils::openDescriptionFile(
    questsDescriptionPath, "quests");

  pugi::xml_node questsListNode = std::get<1>(questsDescription);

  for (pugi::xml_node questNode : questsListNode.children("quest")) {
    std::string questId = questNode.attribute("id").as_string();

    std::string questName = questNode.child_value("name");
    std::string questDescription = questNode.child_value("description");

    Quest quest(questId, questName, questDescription);

    for (pugi::xml_node taskNode : questNode.child("tasks").children("task")) {
      std::string taskId = taskNode.attribute("id").as_string();

      std::string taskName = taskNode.child_value("name");
      std::string taskDescription = taskNode.child_value("description");

      QuestTask questTask(taskId, taskName, taskDescription);

      auto autostartCondition = loadCondition(taskNode.child("autostart_conditions"));
      questTask.setAutostartCondition(autostartCondition);

      auto activeCondition = loadCondition(taskNode.child("active_conditions"));
      questTask.setActiveCondition(activeCondition);

      auto completeCondition = loadCondition(taskNode.child("complete_conditions"));
      questTask.setCompleteCondition(completeCondition);

      quest.addTask(questTask);
    }

    registerQuest(quest);
  }
}

std::shared_ptr<GameLogicCondition> QuestsSystem::loadCondition(const pugi::xml_node& conditionNode)
{
  if (conditionNode) {
    return m_conditionsManager->buildConditionsTree(conditionNode);
  }
  else {
    return nullptr;
  }
}

void QuestsSystem::activate()
{
  GameWorld* gameWorld = getGameWorld();

  gameWorld->subscribeEventsListener<GameObjectAddComponentEvent<ActorComponent>>(this);
  gameWorld->subscribeEventsListener<AddInfoportionEvent>(this);
  gameWorld->subscribeEventsListener<RemoveInfoportionEvent>(this);
  gameWorld->subscribeEventsListener<InventoryItemActionEvent>(this);
}

void QuestsSystem::deactivate()
{
  GameWorld* gameWorld = getGameWorld();

  gameWorld->unsubscribeEventsListener<InventoryItemActionEvent>(this);
  gameWorld->unsubscribeEventsListener<RemoveInfoportionEvent>(this);
  gameWorld->unsubscribeEventsListener<AddInfoportionEvent>(this);
  gameWorld->unsubscribeEventsListener<GameObjectAddComponentEvent<ActorComponent>>(this);
}

EventProcessStatus QuestsSystem::receiveEvent(const GameObjectAddComponentEvent<ActorComponent>& event)
{
  if (event.gameObject.getName() == PLAYER_OBJECT_NAME) {
    setupActorQuestsState();
  }

  return EventProcessStatus::Processed;
}

EventProcessStatus QuestsSystem::receiveEvent(const AddInfoportionEvent& event)
{
  if (event.getActor() == getActor()) {
    updateQuestsStates();
  }

  return EventProcessStatus::Processed;
}

EventProcessStatus QuestsSystem::receiveEvent(const RemoveInfoportionEvent& event)
{
  if (event.getActor() == getActor()) {
    updateQuestsStates();
  }

  return EventProcessStatus::Processed;
}

EventProcessStatus QuestsSystem::receiveEvent(const InventoryItemActionEvent& event)
{
  if (event.inventoryOwner == getActor()) {
    updateQuestsStates();
  }

  return EventProcessStatus::Processed;
}

void QuestsSystem::updateQuestsStates()
{
  GameObject actorObject = getActor();
  SW_ASSERT(actorObject.isAlive());

  auto actor = actorObject.getComponent<ActorComponent>();

  for (const auto&[questId, quest] : m_questsStorage->getQuests()) {
    ActorQuestState& actorQuestState = actor->getQuestState(questId);

    switch (actorQuestState.getState()) {
      case QuestState::NotStarted:
        updateNotStartedQuest(quest, actorQuestState);
        break;
      case QuestState::Started:
        updateStartedQuest(quest, actorQuestState);
        break;

      case QuestState::Completed:
      case QuestState::Failed:
        // Do nothing
        break;

      default:
        SW_ASSERT(false);
        break;
    }
  }
}

bool QuestsSystem::activateNewQuestTask(const Quest& quest, ActorQuestState& actorQuestState)
{
  SW_ASSERT(quest.getId() == actorQuestState.getQuestId());
  SW_ASSERT(actorQuestState.getState() == QuestState::Started);

  // Reset completed tasks to not started if completed condition
  // are not true now
  for (const auto&[taskId, task] : quest.getTasks()) {
    ActorQuestTaskState& actorTaskState = actorQuestState.getTaskState(taskId);

    if (actorTaskState.getState() == QuestTaskState::Completed) {
      GameLogicCondition* taskCompletedCondition = task.getCompleteCondition();

      if (taskCompletedCondition) {
        if (!taskCompletedCondition->calculateValue()) {
          actorTaskState.setState(QuestTaskState::NotStarted);

          getGameWorld()->emitEvent<QuestTaskCancelledEvent>(
            QuestTaskCancelledEvent(getActor(), quest.getId(), task.getId()));
        }
      }
    }
  }

  for (const auto&[taskId, task] : quest.getTasks()) {
    ActorQuestTaskState& actorTaskState = actorQuestState.getTaskState(taskId);

    if (actorTaskState.getState() != QuestTaskState::NotStarted) {
      continue;
    }

    GameLogicCondition* taskActiveCondition = task.getActiveCondition();

    if (taskActiveCondition) {
      if (taskActiveCondition->calculateValue()) {
        // New task to activate is found

        actorTaskState.setState(QuestTaskState::Started);
        actorQuestState.setCurrentTaskId(task.getId());

        getGameWorld()->emitEvent<QuestTaskStartedEvent>(QuestTaskStartedEvent(getActor(), quest.getId(),
          task.getId()));

        return true;
      }
    }
    else {
      // There is no active conditions, so just start the task
      actorTaskState.setState(QuestTaskState::Started);
      actorQuestState.setCurrentTaskId(task.getId());

      getGameWorld()->emitEvent<QuestTaskStartedEvent>(QuestTaskStartedEvent(getActor(), quest.getId(),
        task.getId()));

      return true;
    }
  }

  return false;
}

void QuestsSystem::updateStartedQuest(const Quest& quest, ActorQuestState& actorQuestState)
{
  SW_ASSERT(actorQuestState.getTaskState(actorQuestState.getCurrentTaskId()).getState() ==
    QuestTaskState::Started);

  // Check for completeness of current active task
  GameLogicCondition* activeTaskCompleteCondition =
    quest.getTask(actorQuestState.getCurrentTaskId()).getCompleteCondition();

  bool questIsFinished = false;

  if (activeTaskCompleteCondition) {
    if (activeTaskCompleteCondition->calculateValue()) {
      actorQuestState.getTaskState(actorQuestState.getCurrentTaskId()).setState(QuestTaskState::Completed);

      getGameWorld()->emitEvent<QuestTaskCompletedEvent>(
        QuestTaskCompletedEvent(getActor(), quest.getId(), actorQuestState.getCurrentTaskId()));

      bool isNewTaskActivated = activateNewQuestTask(quest, actorQuestState);

      if (!isNewTaskActivated) {
        // New active task is not found, so complete the quest

        actorQuestState.setState(QuestState::Completed);
        getGameWorld()->emitEvent<QuestCompletedEvent>(QuestCompletedEvent(getActor(), quest.getId()));
        questIsFinished = true;
      }
      else {
        updateStartedQuest(quest, actorQuestState);
      }
    }
  }

  if (!questIsFinished) {
    GameLogicCondition* taskActiveCondition = quest.getTask(actorQuestState.getCurrentTaskId()).getActiveCondition();

    if (taskActiveCondition) {
      if (!taskActiveCondition->calculateValue()) {
        actorQuestState.getTaskState(actorQuestState.getCurrentTaskId()).setState(QuestTaskState::NotStarted);

        getGameWorld()->emitEvent<QuestTaskCancelledEvent>(
          QuestTaskCancelledEvent(getActor(), quest.getId(), actorQuestState.getCurrentTaskId()));

        // Try to switch active quest task to another
        bool isNewTaskActivated = activateNewQuestTask(quest, actorQuestState);

        if (!isNewTaskActivated) {
          THROW_EXCEPTION(EngineRuntimeException,
            fmt::format("New active task for the quest {} is not found", quest.getId()));
        }
        else {
          updateStartedQuest(quest, actorQuestState);
        }
      }
    }
  }
}

void QuestsSystem::updateNotStartedQuest(const Quest& quest, ActorQuestState& actorQuestState)
{
  for (const auto&[questTaskId, questTask] : quest.getTasks()) {
    ActorQuestTaskState& actorTaskState = actorQuestState.getTaskState(questTaskId);

    SW_ASSERT(actorTaskState.getState() == QuestTaskState::NotStarted);

    GameLogicCondition* taskAutostartCondition = questTask.getAutostartCondition();

    if (taskAutostartCondition) {
      if (taskAutostartCondition->calculateValue()) {
        // Start quest and task

        actorQuestState.setState(QuestState::Started);
        actorTaskState.setState(QuestTaskState::Started);

        actorQuestState.setCurrentTaskId(questTask.getId());

        getGameWorld()->emitEvent<QuestStartedEvent>(QuestStartedEvent(getActor(), quest.getId()));
        getGameWorld()->emitEvent<QuestTaskStartedEvent>(
          QuestTaskStartedEvent(getActor(), quest.getId(), actorQuestState.getCurrentTaskId()));

        updateStartedQuest(quest, actorQuestState);
        break;
      }
    }
  }
}

GameObject QuestsSystem::getActor()
{
  return getGameWorld()->findGameObject(PLAYER_OBJECT_NAME);
}

void QuestsSystem::setupActorQuestsState()
{
  for (const auto& taskIt : m_questsStorage->getQuests()) {
    setupActorQuestState(taskIt.second);
  }
}

void QuestsSystem::setupActorQuestState(const Quest& quest)
{
  auto actorComponent = getActor().getComponent<ActorComponent>();

  actorComponent->addQuestState(quest.getId());
  auto& actorQuestState = actorComponent->getQuestState(quest.getId());

  for (const auto&[taskId, task] : quest.getTasks()) {
    actorQuestState.addTaskState(taskId);

    setupConditionsActor(task.getActiveCondition());
    setupConditionsActor(task.getAutostartCondition());
    setupConditionsActor(task.getCompleteCondition());
  }
}

std::shared_ptr<GameLogicConditionsManager> QuestsSystem::getConditionsManager() const
{
  return m_conditionsManager;
}

void QuestsSystem::setupConditionsActor(GameLogicCondition* condition)
{
  m_conditionsManager->setupConditionCommunicators(condition, getActor(), GameObject());
}

std::shared_ptr<QuestsStorage> QuestsSystem::getQuestsStorage() const
{
  return m_questsStorage;
}

void QuestsSystem::registerQuest(const Quest& quest)
{
  m_questsStorage->addQuest(quest);

  if (getActor().isAlive()) {
    setupActorQuestState(quest);
  }
}
