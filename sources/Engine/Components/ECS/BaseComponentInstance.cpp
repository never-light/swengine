#include "BaseComponentInstance.h"

BaseComponentInstance::BaseComponentInstance(GameObject * gameObject)
	: m_gameObject(gameObject)
{
}

BaseComponentInstance::~BaseComponentInstance()
{
}

GameObject * BaseComponentInstance::getGameObject() const
{
	return m_gameObject;
}
