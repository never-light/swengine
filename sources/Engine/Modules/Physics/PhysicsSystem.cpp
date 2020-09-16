#include "precompiled.h"

#pragma hdrstop

#include "PhysicsSystem.h"

#include <utility>
#include "PhysicsBackendFactory.h"

PhysicsSystem::PhysicsSystem() = default;

PhysicsSystem::~PhysicsSystem()
{
  SW_ASSERT(m_physicsBackend == nullptr);
}

void PhysicsSystem::configure()
{
  m_physicsBackend = PhysicsBackendFactory::createPhysicsSystem(getGameWorld());
  m_physicsBackend->configure();
}

void PhysicsSystem::unconfigure()
{
  m_physicsBackend->unconfigure();
  m_physicsBackend = nullptr;
}

void PhysicsSystem::update(float delta)
{
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

void PhysicsSystem::render()
{
  m_physicsBackend->render();
}

void PhysicsSystem::setUpdateStepCallback(std::function<void(float)> callback)
{
  m_physicsBackend->setUpdateStepCallback(std::move(callback));
}
