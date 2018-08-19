#include "GameObject.h"

#include <Engine\assertions.h>

GameObject::GameObject()
	: m_gameObjectId(0),
	m_gameObjectUsage(Usage::Dummy),
	m_gameObjectLocation(Location::None),
	m_gameObjectInteractiveMode(InteractiveMode::None),
	m_useCallback(nullptr),
	m_takeCallback(nullptr)
{

}

GameObject::~GameObject() {

}

GameObject::Id GameObject::getGameObjectId() const
{
	return m_gameObjectId;
}

void GameObject::setGameObjectId(GameObject::Id id)
{
	_assert(m_gameObjectId == 0);

	m_gameObjectId = id;
}

void GameObject::resetGameObjectId()
{
	m_gameObjectId = 0;
}

GameObject::Usage GameObject::getGameObjectUsage() const
{
	return m_gameObjectUsage;
}

void GameObject::setGameObjectUsage(Usage usage)
{
	_assert(m_gameObjectUsage == Usage::Dummy);

	m_gameObjectUsage = usage;
}

bool GameObject::isLightSource() const
{
	return m_gameObjectUsage == Usage::LightSource;
}

bool GameObject::isPlayer() const
{
	return m_gameObjectUsage == Usage::Player;
}

GameObject::Location GameObject::getGameObjectLocation() const
{
	return m_gameObjectLocation;
}

void GameObject::setGameObjectLocation(Location location)
{
	m_gameObjectLocation = location;
}

bool GameObject::isLocatedInWorld()
{
	return m_gameObjectLocation == Location::World;
}

void GameObject::setGameObjectInteractiveMode(InteractiveMode mode)
{
	m_gameObjectInteractiveMode = mode;
}

GameObject::InteractiveMode GameObject::getGameObjectInteractiveMode() const
{
	return m_gameObjectInteractiveMode;
}

bool GameObject::isInteractive() const
{
	return m_gameObjectInteractiveMode != InteractiveMode::None;
}

bool GameObject::isUsable() const
{
	return m_gameObjectInteractiveMode == InteractiveMode::Usable;
}

bool GameObject::isTakeable() const
{
	return m_gameObjectInteractiveMode == InteractiveMode::Takeable;
}

void GameObject::setGameObjectInteractiveTitle(const std::string & title)
{
	m_gameObjectInteractiveTitle = title;
}

const std::string & GameObject::getGameObjectInteractiveTitle() const
{
	return m_gameObjectInteractiveTitle;
}

void GameObject::triggerUse()
{
	if (m_useCallback != nullptr)
		m_useCallback(this);
}

void GameObject::setUseCallback(const ActionCallback & callback)
{
	m_useCallback = callback;
}

void GameObject::triggerTake()
{
	if (m_takeCallback != nullptr)
		m_takeCallback(this);
}

void GameObject::setTakeCallback(const ActionCallback & callback)
{
	m_takeCallback = callback;
}