
#include "GameLogicConditionsManager.h"

#include <utility>

#include "Game/Inventory/InventoryComponent.h"

#include "ActorComponent.h"
#include "InfoportionsSystem.h"
#include "DialoguesManager.h"

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

GameLogicConditionHasNotObject::GameLogicConditionHasNotObject(
  GameLogicConditionsManager* conditionsManager,
  std::string objectId)
  : GameLogicActorCondition(conditionsManager),
    m_objectId(std::move(objectId))
{

}

bool GameLogicConditionHasNotObject::calculateValue()
{
  SW_ASSERT(getActor().isAlive());

  return !getActor().getComponent<InventoryComponent>()->hasItem(m_objectId);
}

void GameLogicActorCondition::setActor(const GameObject& actor)
{
  m_actor = actor;
}

GameObject GameLogicActorCondition::getActor() const
{
  return m_actor;
}

void GameLogicActorCondition::setRole(GameLogicCommunicatorRole role)
{
  m_role = role;
}

GameLogicCommunicatorRole GameLogicActorCondition::getRole() const
{
  return m_role;
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
  : GameLogicConditionBooleanBinary(conditionsManager, std::move(conditions))
{

}

bool GameLogicConditionAll::calculateValue()
{
  // TODO: replace with std::ranges
  for (auto& condition : getConditions()) {
    if (!condition->calculateValue()) {
      return false;
    }
  }

  return true;
}

GameLogicConditionAny::GameLogicConditionAny(
  GameLogicConditionsManager* conditionsManager,
  std::vector<std::unique_ptr<GameLogicCondition>> conditions)
  : GameLogicConditionBooleanBinary(conditionsManager, std::move(conditions))
{

}

bool GameLogicConditionAny::calculateValue()
{
  // TODO: replace with std::ranges

  for (auto& condition : getConditions()) {
    if (condition->calculateValue()) {
      return true;
    }
  }

  return false;
}

GameLogicConditionNot::GameLogicConditionNot(
  GameLogicConditionsManager* conditionsManager,
  std::unique_ptr<GameLogicCondition> condition)
  : GameLogicConditionBooleanUnary(conditionsManager, std::move(condition))
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

  GameLogicCondition* condition = nullptr;

  if (nodeName == "has_object") {
    auto hasObjectCondition = new GameLogicConditionHasObject(this, conditionsNode.child_value());
    hasObjectCondition->setRole(getCommunicatorRoleByName(
      conditionsNode.attribute("role").as_string("actor")));

    condition = hasObjectCondition;
  }
  else if (nodeName == "has_not_object") {
    auto hasNotObjectCondition = new GameLogicConditionHasNotObject(this, conditionsNode.child_value());
    hasNotObjectCondition->setRole(getCommunicatorRoleByName(
      conditionsNode.attribute("role").as_string("actor")));

    condition = hasNotObjectCondition;
  }
  else if (nodeName == "has_infoportion") {
    auto hasInfoportionCondition = new GameLogicConditionHasInfoportion(this, conditionsNode.child_value());
    hasInfoportionCondition->setRole(getCommunicatorRoleByName(
      conditionsNode.attribute("role").as_string("actor")));

    condition = hasInfoportionCondition;
  }
  else if (nodeName == "has_not_infoportion") {
    auto hasNotInfoportionCondition = new GameLogicConditionHasNotInfoportion(this, conditionsNode.child_value());
    hasNotInfoportionCondition->setRole(getCommunicatorRoleByName(
      conditionsNode.attribute("role").as_string("actor")));

    condition = hasNotInfoportionCondition;
  }
  else if (nodeName == "all") {
    condition = parseConditionsNodeAll(conditionsNode);
  }
  else if (nodeName == "any") {
    std::vector<std::unique_ptr<GameLogicCondition>> conditions;

    for (pugi::xml_node child : conditionsNode.children()) {
      conditions.emplace_back(parseConditionsNode(child));
    }

    condition = new GameLogicConditionAny(this, std::move(conditions));
  }
  else if (nodeName == "not") {
    auto inverseCondition = std::unique_ptr<GameLogicCondition>(
      parseConditionsNode(conditionsNode.first_child()));

    condition = new GameLogicConditionNot(this, std::move(inverseCondition));
  }
  else {
    SW_ASSERT(false);
  }

  return condition;
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

void GameLogicConditionsManager::traverseConditionsTree(GameLogicCondition* conditionNode,
  const std::function<void(GameLogicCondition*)>& visitor)
{
  visitor(conditionNode);

  if (auto unaryConditionNode = dynamic_cast<GameLogicConditionBooleanUnary*>(conditionNode)) {
    traverseConditionsTree(unaryConditionNode->getCondition(), visitor);
  }
  else if (auto binaryConditionNode = dynamic_cast<GameLogicConditionBooleanBinary*>(conditionNode)) {
    for (auto& condition : binaryConditionNode->getConditions()) {
      traverseConditionsTree(condition.get(), visitor);
    }
  }
  else {
    /* Do nothing special here */
  }
}

GameLogicActionsList GameLogicConditionsManager::buildActionsList(
  pugi::xml_node actionsNode)
{
  GameLogicActionsList actions;

  for (pugi::xml_node actionNode : actionsNode.children()) {
    std::string nodeName = actionNode.name();

    std::shared_ptr<GameLogicAction> action;

    if (nodeName == "add_infoportion") {
      auto addInfoportionAction = std::make_shared<GameLogicActionAddInfoportion>(
        this, actionNode.child_value());

      addInfoportionAction->setRole(getCommunicatorRoleByName(
        actionNode.attribute("role").as_string("actor")));

      action = addInfoportionAction;
    }
    else if (nodeName == "remove_infoportion") {
      auto removeInfoportionAction = std::make_shared<GameLogicActionRemoveInfoportion>(
        this, actionNode.child_value());

      removeInfoportionAction->setRole(getCommunicatorRoleByName(
        actionNode.attribute("role").as_string("actor")));

      action = removeInfoportionAction;
    }
    else if (nodeName == "transfer_item") {
      auto transferItemAction = std::make_shared<GameLogicActionTransferItem>(
        this, actionNode.child_value());

      transferItemAction->setDirection(getCommunicationDirectionByName(
        actionNode.attribute("direction").as_string("to_npc")));

      action = transferItemAction;
    }
    else if (nodeName == "stop_dialogue") {
      auto stopDialogueAction = std::make_shared<GameLogicActionStopDialogue>(this);
      action = stopDialogueAction;
    }
    else {
      SW_ASSERT(false);
    }

    actions.push_back(action);
  }

  return actions;
}

GameLogicCommunicatorRole GameLogicConditionsManager::getCommunicatorRoleByName(const std::string& roleName)
{
  if (roleName == "actor") {
    return GameLogicCommunicatorRole::Actor;
  }
  else if (roleName == "npc") {
    return GameLogicCommunicatorRole::NPCActor;
  }
  else {
    SW_ASSERT(false);

    return GameLogicCommunicatorRole{};
  }
}

GameLogicCommunicationDirection GameLogicConditionsManager::getCommunicationDirectionByName(
  const std::string& directionType)
{
  if (directionType == "to_actor") {
    return GameLogicCommunicationDirection::ToActor;
  }
  else if (directionType == "to_npc") {
    return GameLogicCommunicationDirection::ToNPCActor;
  }
  else {
    SW_ASSERT(false);

    return GameLogicCommunicationDirection{};
  }
}

void GameLogicConditionsManager::setupConditionCommunicators(
  GameLogicCondition* condition,
  GameObject actor,
  GameObject npc)
{
  traverseConditionsTree(condition, [this, actor, npc](GameLogicCondition* condition) {
    if (auto actorCondition = dynamic_cast<GameLogicActorCondition*>(condition)) {
      switch (actorCondition->getRole()) {
        case GameLogicCommunicatorRole::Actor:
          actorCondition->setActor(actor);
          break;

        case GameLogicCommunicatorRole::NPCActor:
          actorCondition->setActor(npc);
          break;

        default:
          SW_ASSERT(false);
      }
    }
  });
}

void GameLogicConditionsManager::setupActionsCommunicators(
  const GameLogicActionsList& actionsList,
  GameObject actor,
  GameObject npc)
{
  for (auto& action : actionsList) {
    GameLogicAction* actionPtr = action.get();

    if (auto actorAction = dynamic_cast<GameLogicActorAction*>(actionPtr)) {
      switch (actorAction->getRole()) {
        case GameLogicCommunicatorRole::Actor:
          actorAction->setActor(actor);
          break;

        case GameLogicCommunicatorRole::NPCActor:
          actorAction->setActor(npc);
          break;

        default:
          SW_ASSERT(false);
      }
    }
    if (auto directedAction = dynamic_cast<GameLogicActionDirected*>(actionPtr)) {
      switch (directedAction->getDirection()) {
        case GameLogicCommunicationDirection::ToActor:
          directedAction->setInitiator(npc);
          directedAction->setTarget(actor);
          break;

        case GameLogicCommunicationDirection::ToNPCActor:
          directedAction->setInitiator(actor);
          directedAction->setTarget(npc);
          break;

        default:
          SW_ASSERT(false);
      }
    }
  }
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

void GameLogicActionDirected::setDirection(GameLogicCommunicationDirection direction)
{
  m_direction = direction;
}

GameLogicCommunicationDirection GameLogicActionDirected::getDirection() const
{
  return m_direction;
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

void GameLogicActorAction::setRole(GameLogicCommunicatorRole role)
{
  m_role = role;
}

GameLogicCommunicatorRole GameLogicActorAction::getRole() const
{
  return m_role;
}

std::vector<std::unique_ptr<GameLogicCondition>>& GameLogicConditionBooleanBinary::getConditions()
{
  return m_conditions;
}

GameLogicCondition* GameLogicConditionBooleanUnary::getCondition()
{
  return m_condition.get();
}

GameLogicActionStopDialogue::GameLogicActionStopDialogue(GameLogicConditionsManager* conditionsManager)
    : GameLogicActorAction(conditionsManager)
{

}

void GameLogicActionStopDialogue::execute()
{
  getConditionsManager()->getGameWorld().emitEvent<StopDialogueCommandEvent>(StopDialogueCommandEvent());
}
