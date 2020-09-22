#include "InventoryComponent.h"

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
