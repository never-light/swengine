#include "InventoryComponent.h"

#include <utility>

void InventoryComponent::addItem(GameObject item)
{
  m_inventoryItems.push_back(item);
}

void InventoryComponent::removeItem(GameObject item)
{
  m_inventoryItems.erase(std::remove(m_inventoryItems.begin(), m_inventoryItems.end(), item),
    m_inventoryItems.end());
}

const std::vector<GameObject>& InventoryComponent::getItems() const
{
  return m_inventoryItems;
}

bool InventoryComponent::hasItem(const std::string& itemId) const
{
  for (GameObject object : m_inventoryItems) {
    if (object.getComponent<InventoryItemComponent>()->getId() == itemId) {
      return true;
    }
  }

  return false;
}

GameObject InventoryComponent::getItem(const std::string& itemId) const
{
  for (GameObject object : m_inventoryItems) {
    if (object.getComponent<InventoryItemComponent>()->getId() == itemId) {
      return object;
    }
  }

  return GameObject();
}

InventoryComponent::BindingParameters InventoryComponent::getBindingParameters() const
{
  std::vector<std::string> itemsNames;

  for (GameObject object : m_inventoryItems) {
    itemsNames.push_back(object.getComponent<InventoryItemComponent>()->getId());
  }

  return InventoryComponent::BindingParameters{.itemsNames=itemsNames};
}

InventoryComponentBinder::InventoryComponentBinder(const ComponentBindingParameters& componentParameters,
  std::shared_ptr<GameWorld> gameWorld)
  : m_bindingParameters(componentParameters),
    m_gameWorld(std::move(gameWorld))
{

}

void InventoryComponentBinder::bindToObject(GameObject& gameObject)
{
  gameObject.addComponent<InventoryComponent>();

  for (const std::string& itemObjectName : m_bindingParameters.itemsNames) {
    GameObject itemObject = m_gameWorld->findGameObject(itemObjectName);

    if (!itemObject.isAlive()) {
      THROW_EXCEPTION(EngineRuntimeException,
        fmt::format("Inventory item object {} is not alive at the loading time", itemObjectName));
    }

    m_gameWorld->emitEvent<InventoryItemActionCommandEvent>(
      {gameObject,
        InventoryItemActionTriggerType::RelocateToInventory,
        itemObject});
  }

}
