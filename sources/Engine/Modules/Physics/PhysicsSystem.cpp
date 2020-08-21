#include "precompiled.h"

#pragma hdrstop

#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem()
{

}

PhysicsSystem::~PhysicsSystem()
{
  SW_ASSERT(m_physicsBackend == nullptr);
}

void PhysicsSystem::configure(GameWorld* gameWorld)
{
  m_physicsBackend = std::make_unique<PhysicsBackend>(gameWorld->shared_from_this());
  m_physicsBackend->configure();
}

void PhysicsSystem::unconfigure(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

  m_physicsBackend->unconfigure();
  m_physicsBackend = nullptr;
}

void PhysicsSystem::update(GameWorld* gameWorld, float delta)
{
  ARG_UNUSED(gameWorld);

  m_physicsBackend->update(delta);
}

void PhysicsSystem::setGravity(const glm::vec3& gravity)
{
  m_physicsBackend->setGravity(gravity);
}

glm::vec3 PhysicsSystem::getGravity() const
{
  return m_physicsBackend->getGravity();
}
