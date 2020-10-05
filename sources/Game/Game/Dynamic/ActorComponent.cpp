#include "ActorComponent.h"

#include <utility>

ActorDialogue::ActorDialogue(std::string dialogueId, bool isStartedByNCP)
  : m_dialogueId(std::move(dialogueId)),
    m_isStartedByNCP(isStartedByNCP)
{

}

void ActorComponent::setName(const std::string& name)
{
  m_name = name;
}

const std::string& ActorComponent::getName() const
{
  return m_name;
}

const std::vector<ActorDialogue>& ActorComponent::getDialogues() const
{
  return m_dialogues;
}

bool ActorComponent::hasDialogue(const std::string& dialogueId) const
{
  for (const auto& actorDialogue : m_dialogues) {
    if (actorDialogue.getDialogueId() == dialogueId) {
      return true;
    }
  }

  return false;
}

void ActorComponent::addDialogue(const ActorDialogue& dialogue)
{
  m_dialogues.push_back(dialogue);
}

ActorInfoportionsStorage& ActorComponent::getInfoportionsStorage()
{
  return m_infoportionsStorage;
}

const ActorInfoportionsStorage& ActorComponent::getInfoportionsStorage() const
{
  return m_infoportionsStorage;
}

void ActorComponent::addQuestState(const std::string& questId)
{
  m_questsStates.insert({ questId, ActorQuestState(questId) });
}

ActorQuestState& ActorComponent::getQuestState(const std::string& questId)
{
  return m_questsStates.at(questId);
}

const ActorQuestState* ActorComponent::getAnyActiveQuest() const
{
  for (const auto& questIt : m_questsStates) {
    if (questIt.second.getState() == QuestState::Started) {
      return &m_questsStates.at(questIt.first);
    }
  }

  return nullptr;
}

void ActorComponent::setHealth(float health)
{
  m_currentHealth = health;
}

float ActorComponent::getHealth() const
{
  return m_currentHealth;
}

void ActorComponent::setHealthLimit(float health)
{
  m_maxHealth = health;
}

float ActorComponent::getHealthLimit() const
{
  return m_maxHealth;
}

void ActorComponent::setDead(bool isDead)
{
  m_isDead = isDead;
}

bool ActorComponent::isDead() const
{
  return m_isDead;
}

const std::string& ActorDialogue::getDialogueId() const
{
  return m_dialogueId;
}

bool ActorDialogue::isStartedByNPC() const
{
  return m_isStartedByNCP;
}

ActorQuestTaskState::ActorQuestTaskState(std::string taskId)
  : m_taskId(std::move(taskId))
{

}

const std::string& ActorQuestTaskState::getTaskId() const
{
  return m_taskId;
}

void ActorQuestTaskState::setState(QuestTaskState state)
{
  m_taskState = state;
}

[[nodiscard]] QuestTaskState ActorQuestTaskState::getState() const
{
  return m_taskState;
}

ActorQuestState::ActorQuestState(std::string questId)
  : m_questId(std::move(questId))
{

}

void ActorQuestState::addTaskState(const std::string& taskId)
{
  m_tasksStates.insert({ taskId, ActorQuestTaskState(taskId) });
}

ActorQuestTaskState& ActorQuestState::getTaskState(const std::string& taskId)
{
  return m_tasksStates.at(taskId);
}

const std::string& ActorQuestState::getQuestId() const
{
  return m_questId;
}

void ActorQuestState::setState(QuestState state)
{
  m_questState = state;
}

QuestState ActorQuestState::getState() const
{
  return m_questState;
}

const std::string& ActorQuestState::getCurrentTaskId() const
{
  return m_currentTaskId;
}

void ActorQuestState::setCurrentTaskId(const std::string& taskId)
{
  SW_ASSERT(m_tasksStates.contains(taskId));

  m_currentTaskId = taskId;
}
