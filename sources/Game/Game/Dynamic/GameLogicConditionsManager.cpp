
#include "GameLogicConditionsManager.h"

#include <utility>

#include "Game/Inventory/InventoryComponent.h"

#include "ActorComponent.h"
#include "InfoportionsSystem.h"

GameLogicConditionHasObject::GameLogicConditionHasObject(
  GameLogicConditionsManager* conditionsManager,
  std::string objectId)
  : GameLogicActorCondition(conditionsManager),
    m_objectId(std::move(objectId))
{

}

bool GameLogicConditionHasObject::calculateValue()
{
  SW_ASSERT(getActor().isAlive());

  return getActor().getComponent<InventoryComponent>()->hasItem(m_objectId);
}

void GameLogicActorCondition::setActor(const GameObject& actor)
{
  m_actor = actor;
}

GameObject GameLogicActorCondition::getActor() const
{
  return m_actor;
}

GameLogicConditionsManager::GameLogicConditionsManager(std::shared_ptr<GameWorld> gameWorld)
  : m_gameWorld(std::move(gameWorld))
{

}

GameLogicConditionHasInfoportion::GameLogicConditionHasInfoportion(
  GameLogicConditionsManager* conditionsManager,
  std::string infoportionName)
  : GameLogicActorCondition(conditionsManager),
    m_infoportionName(std::move(infoportionName))
{

}

bool GameLogicConditionHasInfoportion::calculateValue()
{
  SW_ASSERT(getActor().isAlive());

  return getActor().getComponent<ActorComponent>()->
    getInfoportionsStorage().hasInfoportion(m_infoportionName);
}

GameLogicConditionHasNotInfoportion::GameLogicConditionHasNotInfoportion(
  GameLogicConditionsManager* conditionsManager,
  std::string infoportionName)
  : GameLogicActorCondition(conditionsManager),
    m_infoportionName(std::move(infoportionName))
{

}

bool GameLogicConditionHasNotInfoportion::calculateValue()
{
  SW_ASSERT(getActor().isAlive());

  return !getActor().getComponent<ActorComponent>()->
    getInfoportionsStorage().hasInfoportion(m_infoportionName);
}

GameLogicConditionAll::GameLogicConditionAll(
  GameLogicConditionsManager* conditionsManager,
  std::vector<std::unique_ptr<GameLogicCondition>> conditions)
  : GameLogicCondition(conditionsManager),
    m_conditions(std::move(conditions))
{

}

bool GameLogicConditionAll::calculateValue()
{
  // TODO: replace with std::ranges
  for (auto& condition : m_conditions) {
    if (!condition->calculateValue()) {
      return false;
    }
  }

  return true;
}

GameLogicConditionAny::GameLogicConditionAny(
  GameLogicConditionsManager* conditionsManager,
  std::vector<std::unique_ptr<GameLogicCondition>> conditions)
  : GameLogicCondition(conditionsManager),
    m_conditions(std::move(conditions))
{

}

bool GameLogicConditionAny::calculateValue()
{
  // TODO: replace with std::ranges

  for (auto& condition : m_conditions) {
    if (condition->calculateValue()) {
      return true;
    }
  }

  return false;
}

GameLogicConditionNot::GameLogicConditionNot(
  GameLogicConditionsManager* conditionsManager,
  std::unique_ptr<GameLogicCondition> condition)
  : GameLogicCondition(conditionsManager),
    m_condition(std::move(condition))
{

}

bool GameLogicConditionNot::calculateValue()
{
  return !m_condition->calculateValue();
}

GameLogicActionAddInfoportion::GameLogicActionAddInfoportion(
  GameLogicConditionsManager* conditionsManager,
  std::string infoportionName)
  : GameLogicActorAction(conditionsManager),
    m_infoportionName(std::move(infoportionName))
{

}

void GameLogicActionAddInfoportion::execute()
{
  SW_ASSERT(getActor().isAlive());

  getConditionsManager()->getGameWorld().emitEvent(
    AddInfoportionCommandEvent(getActor(), m_infoportionName));
}

GameLogicActionRemoveInfoportion::GameLogicActionRemoveInfoportion(
  GameLogicConditionsManager* conditionsManager,
  std::string infoportionName)
  : GameLogicActorAction(conditionsManager),
    m_infoportionName(std::move(infoportionName))
{

}

void GameLogicActionRemoveInfoportion::execute()
{
  SW_ASSERT(getActor().isAlive());

  getConditionsManager()->getGameWorld().emitEvent(
    RemoveInfoportionCommandEvent(getActor(), m_infoportionName));
}

GameObject GameLogicConditionsManager::getPlayer() const
{
  return m_gameWorld->findGameObject("player");
}

GameLogicCondition* GameLogicConditionsManager::parseConditionsNode(pugi::xml_node conditionsNode)
{
  std::string nodeName = conditionsNode.name();

  if (nodeName == "has_object") {
    return new GameLogicConditionHasObject(this, conditionsNode.child_value());
  }
  else if (nodeName == "has_info") {
    return new GameLogicConditionHasInfoportion(this, conditionsNode.child_value());
  }
  else if (nodeName == "has_not_info") {
    return new GameLogicConditionHasNotInfoportion(this, conditionsNode.child_value());
  }
  else if (nodeName == "all") {
    return parseConditionsNodeAll(conditionsNode);
  }
  else if (nodeName == "any") {
    std::vector<std::unique_ptr<GameLogicCondition>> conditions;

    for (pugi::xml_node child : conditionsNode.children()) {
      conditions.emplace_back(parseConditionsNode(child));
    }

    return new GameLogicConditionAny(this, std::move(conditions));
  }
  else {
    SW_ASSERT(false);
    return nullptr;
  }
}

GameLogicCondition* GameLogicConditionsManager::parseConditionsNodeAll(pugi::xml_node conditionsNode)
{
  std::vector<std::unique_ptr<GameLogicCondition>> conditions;

  for (pugi::xml_node child : conditionsNode.children()) {
    conditions.emplace_back(parseConditionsNode(child));
  }

  return new GameLogicConditionAll(this, std::move(conditions));
}

std::shared_ptr<GameLogicCondition> GameLogicConditionsManager::buildConditionsTree(pugi::xml_node conditionsNode)
{
  return std::shared_ptr<GameLogicCondition>(parseConditionsNodeAll(conditionsNode));
}

GameWorld& GameLogicConditionsManager::getGameWorld()
{
  return *m_gameWorld;
}

GameLogicActionDirected::GameLogicActionDirected(GameLogicConditionsManager* conditionsManager)
  : GameLogicAction(conditionsManager)
{

}

void GameLogicActionDirected::setInitiator(GameObject initiator)
{
  m_initiator = initiator;
}

GameObject GameLogicActionDirected::getInitiator() const
{
  return m_initiator;
}

void GameLogicActionDirected::setTarget(GameObject target)
{
  m_target = target;
}

GameObject GameLogicActionDirected::getTarget() const
{
  return m_target;
}

GameLogicActionTransferItem::GameLogicActionTransferItem(
  GameLogicConditionsManager* conditionsManager,
  std::string itemName)
  : GameLogicActionDirected(conditionsManager),
    m_itemName(std::move(itemName))
{

}

void GameLogicActionTransferItem::execute()
{
  auto initiatorInventory = getInitiator().getComponent<InventoryComponent>();

  getConditionsManager()->getGameWorld().emitEvent<InventoryItemTransferCommandEvent>(
    InventoryItemTransferCommandEvent(getInitiator(),
      getTarget(),
      initiatorInventory->getItem(m_itemName)));
}

void GameLogicActionTransferItem::setItemName(const std::string& name)
{
  m_itemName = name;
}

const std::string& GameLogicActionTransferItem::getItemName() const
{
  return m_itemName;
}

void GameLogicActorAction::setActor(const GameObject& actor)
{
  m_actor = actor;
}

GameObject GameLogicActorAction::getActor() const
{
  return m_actor;
}
