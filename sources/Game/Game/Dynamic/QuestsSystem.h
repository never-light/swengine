#pragma once

#include <unordered_map>

#include "GameLogicConditionsManager.h"
#include "Quest.h"

class QuestsSystem {
 public:
  explicit QuestsSystem(std::shared_ptr<GameLogicConditionsManager> conditionsManager);

  void addQuest(const Quest& quest);
  [[nodiscard]] const Quest& getQuest(const std::string& questId) const;

  [[nodiscard]] bool hasQuest(const std::string& questId) const;

  void loadFromFile(const std::string& path);

 private:
  std::shared_ptr<GameLogicCondition> loadCondition(const pugi::xml_node& conditionNode);

 private:
  std::unordered_map<std::string, Quest> m_quests;

  std::shared_ptr<GameLogicConditionsManager> m_conditionsManager;
};
