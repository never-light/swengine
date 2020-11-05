#include "GameComponentsLoader.h"

#include <Engine/Modules/Graphics/Resources/TextureResourceManager.h>
#include <Engine/Exceptions/exceptions.h>

#include <utility>

#include "Game/PlayerComponent.h"
#include "Game/Inventory/InventoryComponent.h"
#include "Game/Dynamic/InteractiveObjectComponent.h"
#include "Game/Dynamic/ActorComponent.h"

GameComponentsLoader::GameComponentsLoader(
  std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<ResourcesManager> resourceManager)
  : m_gameWorld(std::move(gameWorld)),
    m_resourceManager(std::move(std::move(resourceManager)))
{

}

std::unique_ptr<GameObjectsComponentBinder> GameComponentsLoader::loadPlayerData(
  const pugi::xml_node& data)
{
  PlayerComponentBindingParameters bindingParameters;

  float height = data.attribute("height").as_float(1.0f);
  bindingParameters.setHeight(height);

  float walk_speed = data.attribute("walk_speed").as_float(1.0f);
  bindingParameters.setWalkSpeed(walk_speed);

  return std::make_unique<PlayerComponentBinder>(bindingParameters);
}

std::unique_ptr<GameObjectsComponentBinder> GameComponentsLoader::loadInventoryItemData(const pugi::xml_node& data)
{
  InventoryItemComponentBindingParameters bindingParameters;

  std::string itemName = data.attribute("name").as_string();
  bindingParameters.setName(itemName);

  std::string itemTitle = data.attribute("title").as_string();
  bindingParameters.setTitle(itemTitle);

  std::string iconName = data.attribute("icon").as_string();
  bindingParameters.setIconResourceName(iconName);

  bindingParameters.setIsReadable(data.attribute("readable").as_bool());
  bindingParameters.setIsUsable(data.attribute("usable").as_bool());
  bindingParameters.setIsDroppable(data.attribute("droppable").as_bool());

  bindingParameters.setShortDescription(data.child_value("short_desc"));
  bindingParameters.setLongDescription(data.child_value("long_desc"));

  return std::make_unique<InventoryItemComponentBinder>(bindingParameters, m_resourceManager);
}

std::unique_ptr<GameObjectsComponentBinder> GameComponentsLoader::loadInventoryData(const pugi::xml_node& data)
{
  InventoryComponentBindingParameters bindingParameters;

  for (pugi::xml_node itemNode : data.child("items").children("item")) {
    std::string itemObjectName = itemNode.attribute("name").as_string();

    bindingParameters.addItemName(itemObjectName);
  }

  return std::make_unique<InventoryComponentBinder>(bindingParameters, m_gameWorld);
}

std::unique_ptr<GameObjectsComponentBinder> GameComponentsLoader::loadInteractiveData(const pugi::xml_node& data)
{
  InteractiveComponentBindingParameters bindingParameters;

  std::string objectName = data.attribute("name").as_string();
  bindingParameters.setTitle(objectName);

  pugi::xml_node takeableConditions = data.child("takeable");

  if (takeableConditions) {
    bindingParameters.setIsTakeable(true);
  }

  pugi::xml_node usableConditions = data.child("usable");

  if (usableConditions) {
    bindingParameters.setIsUsable(true);
  }

  pugi::xml_node talkableConditions = data.child("talkable");

  if (talkableConditions) {
    bindingParameters.setIsTalkable(true);
  }

  return std::make_unique<InteractiveComponentBinder>(bindingParameters);
}

std::unique_ptr<GameObjectsComponentBinder> GameComponentsLoader::loadActorData(const pugi::xml_node& data)
{
  ActorComponentBindingParameters bindingParameters;

  std::string actorName = data.attribute("name").as_string();
  bindingParameters.setName(actorName);

  pugi::xml_node dialoguesNode = data.child("dialogues");

  for (pugi::xml_node dialogueNode : dialoguesNode.children("dialogue")) {
    std::string dialogueId = dialogueNode.attribute("id").as_string();
    bool isStartedByNPC = dialogueNode.attribute("npc_start").as_bool(false);

    bindingParameters.addDialogue(dialogueId, isStartedByNPC);
  }

  pugi::xml_node healthNode = data.child("health");

  if (healthNode) {
    bindingParameters.setHealth(healthNode.attribute("value").as_float(0.0f));
    bindingParameters.setHealthLimit(healthNode.attribute("limit").as_float(100.0f));
  }

  return std::make_unique<ActorComponentBinder>(bindingParameters);
}
