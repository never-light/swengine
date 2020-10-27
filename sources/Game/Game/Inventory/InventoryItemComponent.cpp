#include "InventoryItemComponent.h"

#include <utility>

InventoryItemComponent::InventoryItemComponent(
  ResourceHandle<GLTexture> icon,
  std::string id,
  std::string name)
  : m_icon(std::move(icon)),
    m_id(std::move(id)),
    m_name(std::move(name))
{

}

void InventoryItemComponent::setIcon(ResourceHandle<GLTexture> icon)
{
  m_icon = icon;
}

ResourceHandle<GLTexture> InventoryItemComponent::getIcon() const
{
  return m_icon;
}

void InventoryItemComponent::setId(const std::string& id)
{
  m_id = id;
}

const std::string& InventoryItemComponent::getId() const
{
  return m_id;
}

void InventoryItemComponent::setName(const std::string& name)
{
  m_name = name;
}

const std::string& InventoryItemComponent::getName() const
{
  return m_name;
}

void InventoryItemComponent::setShortDescription(const std::string& description)
{
  m_shortDescription = description;
}

const std::string& InventoryItemComponent::getShortDescription() const
{
  return m_shortDescription;
}

void InventoryItemComponent::setLongDescription(const std::string& description)
{
  m_longDescription = description;
}

const std::string& InventoryItemComponent::getLongDescription() const
{
  return m_longDescription;
}

void InventoryItemComponent::setReadable(bool isReadable)
{
  m_isReadable = isReadable;
}

bool InventoryItemComponent::isReadable() const
{
  return m_isReadable;
}

void InventoryItemComponent::setUsable(bool isUsable)
{
  m_isUsable = isUsable;
}

bool InventoryItemComponent::isUsable() const
{
  return m_isUsable;
}

void InventoryItemComponent::setDroppable(bool isDroppable)
{
  m_isDroppable = isDroppable;
}

bool InventoryItemComponent::isDroppable() const
{
  return m_isDroppable;
}

const GameObject& InventoryItemComponent::getOwner() const
{
  return m_owner;
}

void InventoryItemComponent::setOwner(const GameObject& owner)
{
  m_owner = owner;
}

void InventoryItemComponent::setUseCallback(const InventoryItemComponent::ActionCallback& callback)
{
  m_useCallback = callback;
}

InventoryItemComponent::ActionCallback InventoryItemComponent::getUseCallback() const
{
  return m_useCallback;
}

void InventoryItemComponent::setDropCallback(const InventoryItemComponent::ActionCallback& callback)
{
  m_dropCallback = callback;
}

InventoryItemComponent::ActionCallback InventoryItemComponent::getDropCallback() const
{
  return m_dropCallback;
}

void InventoryItemComponent::setTakeCallback(const InventoryItemComponent::ActionCallback& callback)
{
  m_takeCallback = callback;
}

InventoryItemComponent::ActionCallback InventoryItemComponent::getTakeCallback() const
{
  return m_takeCallback;
}

void InventoryItemComponent::setReadCallback(const InventoryItemComponent::ActionCallback& callback)
{
  m_readCallback = callback;
}

InventoryItemComponent::ActionCallback InventoryItemComponent::getReadCallback() const
{
  return m_readCallback;
}
