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

GameLogicCondition& QuestTask::getAutostartCondition()
{
  return *m_autostartCondition;
}

GameLogicCondition& QuestTask::getActiveCondition()
{
  return *m_activeCondition;
}

GameLogicCondition& QuestTask::geCompleteCondition()
{
  return *m_completeCondition;
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
  m_tasks.push_back(task);
}

const std::vector<QuestTask>& Quest::getTasks() const
{
  return m_tasks;
}

