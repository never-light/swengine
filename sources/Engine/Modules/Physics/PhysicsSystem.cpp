#include "precompiled.h"

#pragma hdrstop

#include "PhysicsSystem.h"
#include "PhysicsBackendFactory.h"

PhysicsSystem::PhysicsSystem()
{

}

PhysicsSystem::~PhysicsSystem()
{
  SW_ASSERT(m_physicsBackend == nullptr);
}

void PhysicsSystem::configure(GameWorld* gameWorld)
{
  m_physicsBackend = PhysicsBackendFactory::createPhysicsSystem(gameWorld->shared_from_this());
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

void PhysicsSystem::enableDebugDrawing(bool enable)
{
  m_physicsBackend->enableDebugDrawing(enable);
}

bool PhysicsSystem::isDebugDrawingEnabled()
{
  return m_physicsBackend->isDebugDrawingEnabled();
}

void PhysicsSystem::render(GameWorld* gameWorld)
{
  ARG_UNUSED(gameWorld);

  m_physicsBackend->render();
}
