#include "GameComponentsLoader.h"

#include <Engine/Modules/Graphics/Resources/TextureResource.h>
#include <Engine/Exceptions/exceptions.h>

#include <utility>

#include "Game/PlayerComponent.h"
#include "Game/Inventory/InventoryComponent.h"

GameComponentsLoader::GameComponentsLoader(
  std::shared_ptr<GameWorld> gameWorld,
  std::shared_ptr<ResourceManager> resourceManager)
  : m_gameWorld(std::move(gameWorld)),
    m_resourceManager(std::move(std::move(resourceManager)))
{

}

void GameComponentsLoader::loadPlayerData(GameObject& gameObject,
  const pugi::xml_node& data)
{
  float height = data.attribute("height").as_float(1.0f);

  auto& playerComponent = *gameObject.addComponent<PlayerComponent>(height).get();

  float walk_speed = data.attribute("walk_speed").as_float(1.0f);
  playerComponent.setMovementSpeed(walk_speed);
}

void GameComponentsLoader::loadInventoryItemData(GameObject& gameObject, const pugi::xml_node& data)
{
  std::string itemName = data.attribute("name").as_string();

  std::string iconName = data.attribute("icon").as_string();
  auto iconTexture = m_resourceManager->getResourceFromInstance<TextureResource>(iconName)->getTexture();

  auto& inventoryItemComponent = *gameObject.addComponent<InventoryItemComponent>(iconTexture, itemName).get();

  inventoryItemComponent.setReadable(data.attribute("readable").as_bool());
  inventoryItemComponent.setUsable(data.attribute("usable").as_bool());
  inventoryItemComponent.setDroppable(data.attribute("droppable").as_bool());

  inventoryItemComponent.setShortDescription(data.child_value("short_desc"));
  inventoryItemComponent.setLongDescription(data.child_value("long_desc"));
}

void GameComponentsLoader::loadInventoryData(GameObject& gameObject, const pugi::xml_node& data)
{
  gameObject.addComponent<InventoryComponent>().get();

  for (pugi::xml_node itemNode : data.child("items").children("item")) {
    std::string itemObjectName = itemNode.attribute("name").as_string();

    GameObject itemObject = m_gameWorld->findGameObject(itemObjectName);

    if (!itemObject.isAlive()) {
      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("Inventory item object {} is not alive at the loading time", itemObjectName));
    }

    m_gameWorld->emitEvent<InventoryItemActionTriggeredEvent>(
      {gameObject,
        InventoryItemActionTriggerType::RelocateToInventory,
        itemObject});
  }
}
