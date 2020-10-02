#include "QuestsSystem.h"

#include <utility>

#include <Engine/Utility/files.h>
#include <Engine/Utility/xml.h>

QuestsSystem::QuestsSystem(std::shared_ptr<GameLogicConditionsManager> conditionsManager)
  : m_conditionsManager(std::move(conditionsManager))
{

}

void QuestsSystem::addQuest(const Quest& quest)
{
  m_quests.insert({quest.getId(), quest});
}

const Quest& QuestsSystem::getQuest(const std::string& questId) const
{
  return m_quests.at(questId);
}

bool QuestsSystem::hasQuest(const std::string& questId) const
{
  return m_quests.contains(questId);
}

void QuestsSystem::loadFromFile(const std::string& path)
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

    addQuest(quest);
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
