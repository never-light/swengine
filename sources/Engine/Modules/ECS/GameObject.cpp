#include "precompiled.h"

#pragma hdrstop

#include "GameObject.h"

GameObject::GameObject(GameObjectId id, GameWorld* gameWorld)
  : m_id(id),
    m_isDestroyed(false),
    m_gameWorld(gameWorld)
{

}

GameObject::GameObject(GameObjectId id, const std::string& name, GameWorld* gameWorld)
  : m_id(id),
    m_isDestroyed(false),
    m_name(name),
    m_gameWorld(gameWorld)
{

}

GameObject::~GameObject() = default;

GameObjectId GameObject::getId() const
{
  return m_id;
}

const std::string& GameObject::getName() const
{
  return m_name;
}

bool GameObject::isAlive() const
{
  return !m_isDestroyed;
}
