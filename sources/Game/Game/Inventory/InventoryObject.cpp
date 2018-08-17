#include "InventoryObject.h"

InventoryObject::InventoryObject(Texture* icon)
	: m_inventoryIcon(icon),
	m_useCallback(nullptr),
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

void InventoryObject::triggerUse()
{
	if (m_useCallback != nullptr)
		m_useCallback(this);
}

void InventoryObject::setUseCallback(const ActionCallback & callback)
{
	m_useCallback = callback;
}

void InventoryObject::triggerTake()
{
	if (m_takeCallback != nullptr)
		m_takeCallback(this);
}

void InventoryObject::setTakeCallback(const ActionCallback & callback)
{
	m_takeCallback = callback;
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
