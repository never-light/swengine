#include "InventoryObject.h"

InventoryObject::InventoryObject(Texture* icon)
	: m_inventoryIcon(icon),
	m_inventoryUsage(InventoryUsage::Simple)
{
}

InventoryObject::~InventoryObject()
{
}

Texture * InventoryObject::getInventoryIcon() const
{
	return m_inventoryIcon;
}

const std::string & InventoryObject::getInventoryTitle() const
{
	return m_inventoryTitle;
}

void InventoryObject::setInventoryTitle(const std::string & title)
{
	m_inventoryTitle = title;
}

void InventoryObject::setInventoryUsage(InventoryUsage usage)
{
	m_inventoryUsage = usage;
}

InventoryObject::InventoryUsage InventoryObject::getInventoryUsage() const
{
	return m_inventoryUsage;
}
