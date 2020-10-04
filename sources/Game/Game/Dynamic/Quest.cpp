#include "Quest.h"

#include <utility>

QuestTask::QuestTask(std::string id, std::string name, std::string description)
  : m_id(std::move(id)),
    m_name(std::move(name)),
    m_description(std::move(description))
{

}

const std::string& QuestTask::getName() const
{
  return m_name;
}

const std::string& QuestTask::getDescription() const
{
  return m_description;
}

const std::string& QuestTask::getId() const
{
  return m_id;
}

void QuestTask::setAutostartCondition(std::shared_ptr<GameLogicCondition> condition)
{
  m_autostartCondition = std::move(condition);
}

void QuestTask::setActiveCondition(std::shared_ptr<GameLogicCondition> condition)
{
  m_activeCondition = std::move(condition);
}

GameLogicCondition* QuestTask::getAutostartCondition() const
{
  return m_autostartCondition.get();
}

GameLogicCondition* QuestTask::getActiveCondition() const
{
  return m_activeCondition.get();
}

GameLogicCondition* QuestTask::getCompleteCondition() const
{
  return m_completeCondition.get();
}

void QuestTask::setCompleteCondition(std::shared_ptr<GameLogicCondition> condition)
{
  m_completeCondition = std::move(condition);
}

Quest::Quest(std::string id, std::string name, std::string description)
  : m_id(std::move(id)),
    m_name(std::move(name)),
    m_description(std::move(description))
{

}

const std::string& Quest::getId() const
{
  return m_id;
}

const std::string& Quest::getName() const
{
  return m_name;
}

const std::string& Quest::getDescription() const
{
  return m_description;
}

void Quest::addTask(const QuestTask& task)
{
  m_tasks.insert({task.getId(), task});
}

const std::unordered_map<std::string, QuestTask>& Quest::getTasks() const
{
  return m_tasks;
}

const QuestTask& Quest::getTask(const std::string& taskId) const
{
  return m_tasks.at(taskId);
}

