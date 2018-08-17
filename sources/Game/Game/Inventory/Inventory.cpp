#include "Inventory.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::addObject(InventoryObject * object)
{
	m_objects.push_back(object);
}

const std::vector<InventoryObject*>& Inventory::getObjects() const
{
	return m_objects;
}
