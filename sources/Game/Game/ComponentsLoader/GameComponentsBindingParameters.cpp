#include "GameComponentsBindingParameters.h"

float PlayerComponentBindingParameters::getHeight() const
{
  return m_height;
}

void PlayerComponentBindingParameters::setHeight(float height)
{
  m_height = height;
}

float PlayerComponentBindingParameters::getWalkSpeed() const
{
  return m_walkSpeed;
}

void PlayerComponentBindingParameters::setWalkSpeed(float walkSpeed)
{
  m_walkSpeed = walkSpeed;
}

const std::string& ActorComponentBindingParameters::getName() const
{
  return m_name;
}

void ActorComponentBindingParameters::setName(const std::string& name)
{
  m_name = name;
}

float ActorComponentBindingParameters::getHealth() const
{
  return m_health;
}

void ActorComponentBindingParameters::setHealth(float health)
{
  m_health = health;
}

float ActorComponentBindingParameters::getHealthLimit() const
{
  return m_healthLimit;
}

void ActorComponentBindingParameters::setHealthLimit(float healthLimit)
{
  m_healthLimit = healthLimit;
}

const std::vector<std::pair<std::string, bool>>& ActorComponentBindingParameters::getDialoguesList() const
{
  return m_dialoguesList;
}

void ActorComponentBindingParameters::setDialoguesList(const std::vector<std::pair<std::string, bool>>& dialoguesList)
{
  m_dialoguesList = dialoguesList;
}

void ActorComponentBindingParameters::addDialogue(const std::string& dialogueName, bool isStartedByNPC)
{
  m_dialoguesList.emplace_back(dialogueName, isStartedByNPC);
}

const std::string& RigidBodyComponentBindingParameters::getCollisionModelResourceName() const
{
  return m_collisionModelResourceName;
}

void RigidBodyComponentBindingParameters::setCollisionModelResourceName(const std::string& collisionModelResourceName)
{
  m_collisionModelResourceName = collisionModelResourceName;
}

float RigidBodyComponentBindingParameters::getMass() const
{
  return m_mass;
}

void RigidBodyComponentBindingParameters::setMass(float mass)
{
  m_mass = mass;
}

const std::vector<std::string>& InventoryComponentBindingParameters::getItemsNames() const
{
  return m_itemsNames;
}

void InventoryComponentBindingParameters::setItemsNames(const std::vector<std::string>& itemsNames)
{
  m_itemsNames = itemsNames;
}

void InventoryComponentBindingParameters::addItemName(const std::string& itemName)
{
  m_itemsNames.push_back(itemName);
}

bool InteractiveComponentBindingParameters::isTakeable() const
{
  return m_isTakeable;
}

void InteractiveComponentBindingParameters::setIsTakeable(bool isTakeable)
{
  m_isTakeable = isTakeable;
}

bool InteractiveComponentBindingParameters::isTalkable() const
{
  return m_isTalkable;
}

void InteractiveComponentBindingParameters::setIsTalkable(bool isTalkable)
{
  m_isTalkable = isTalkable;
}

const std::string& InteractiveComponentBindingParameters::getTitle() const
{
  return m_title;
}

void InteractiveComponentBindingParameters::setTitle(const std::string& title)
{
  m_title = title;
}

bool InteractiveComponentBindingParameters::isUsable() const
{
  return m_isUsable;
}

void InteractiveComponentBindingParameters::setIsUsable(bool isUsable)
{
  m_isUsable = isUsable;
}

const std::string& InventoryItemComponentBindingParameters::getShortDescription() const
{
  return m_shortDescription;
}

void InventoryItemComponentBindingParameters::setShortDescription(const std::string& shortDescription)
{
  m_shortDescription = shortDescription;
}

const std::string& InventoryItemComponentBindingParameters::getLongDescription() const
{
  return m_longDescription;
}

void InventoryItemComponentBindingParameters::setLongDescription(const std::string& longDescription)
{
  m_longDescription = longDescription;
}

const std::string& InventoryItemComponentBindingParameters::getName() const
{
  return m_name;
}

void InventoryItemComponentBindingParameters::setName(const std::string& name)
{
  m_name = name;
}

const std::string& InventoryItemComponentBindingParameters::getTitle() const
{
  return m_title;
}

void InventoryItemComponentBindingParameters::setTitle(const std::string& title)
{
  m_title = title;
}

const std::string& InventoryItemComponentBindingParameters::getIconResourceName() const
{
  return m_iconResourceName;
}

void InventoryItemComponentBindingParameters::setIconResourceName(const std::string& iconResourceName)
{
  m_iconResourceName = iconResourceName;
}

bool InventoryItemComponentBindingParameters::isReadable() const
{
  return m_isReadable;
}

void InventoryItemComponentBindingParameters::setIsReadable(bool isReadable)
{
  m_isReadable = isReadable;
}

bool InventoryItemComponentBindingParameters::isUsable() const
{
  return m_isUsable;
}

void InventoryItemComponentBindingParameters::setIsUsable(bool isUsable)
{
  m_isUsable = isUsable;
}

bool InventoryItemComponentBindingParameters::isDroppable() const
{
  return m_isDroppable;
}

void InventoryItemComponentBindingParameters::setIsDroppable(bool isDroppable)
{
  m_isDroppable = isDroppable;
}

