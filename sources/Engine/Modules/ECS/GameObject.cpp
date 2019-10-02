#include "GameObject.h"

GameObject::GameObject(GameObjectId id, GameWorld* gameWorld)
	: m_id(id), m_isDestroyed(false), m_gameWorld(gameWorld)
{

}

GameObject::~GameObject() {
}

GameObjectId GameObject::getId() const
{
	return m_id;
}

bool GameObject::isDestroyed() const
{
	return m_isDestroyed;
}
