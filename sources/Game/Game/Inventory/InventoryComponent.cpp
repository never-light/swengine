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
