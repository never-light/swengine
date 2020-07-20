#include "precompiled.h"

#pragma hdrstop

#include "BulletPhysicsAdapter.h"

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Physics/RigidBodyComponent.h"

BulletPhysicsAdapter::BulletPhysicsAdapter(std::weak_ptr<GameWorld> gameWorld)
  : m_gameWorld(gameWorld)
{
}

BulletPhysicsAdapter::~BulletPhysicsAdapter()
{
}

glm::vec3 BulletPhysicsAdapter::getGravity() const
{
  btVector3 gravity = m_dynamicsWorld->getGravity();

  return { gravity.x(), gravity.y(), gravity.z() };
}

void BulletPhysicsAdapter::setGravity(const glm::vec3& gravity)
{
  m_dynamicsWorld->setGravity({ gravity.x, gravity.y, gravity.z });
}

void BulletPhysicsAdapter::update(float delta)
{
  m_dynamicsWorld->stepSimulation(delta);
}

void BulletPhysicsAdapter::configure()
{
  m_collisionConfiguration = new btDefaultCollisionConfiguration();
  m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);
  m_broadphaseInterface = new btDbvtBroadphase();
  m_constraintSolver = new btSequentialImpulseConstraintSolver();

  m_dynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher,
    m_broadphaseInterface, m_constraintSolver, m_collisionConfiguration);

}

void BulletPhysicsAdapter::unconfigure()
{
  delete m_dynamicsWorld;
  m_dynamicsWorld = nullptr;

  delete m_constraintSolver;
  m_constraintSolver = nullptr;

  delete m_broadphaseInterface;
  m_broadphaseInterface = nullptr;

  delete m_collisionDispatcher;
  m_collisionDispatcher = nullptr;

  delete m_collisionConfiguration;
  m_collisionConfiguration = nullptr;
}

EventProcessStatus BulletPhysicsAdapter::receiveEvent(GameWorld* gameWorld,
  const GameObjectRemoveComponentEvent<RigidBodyComponent>& event)
{
  ARG_UNUSED(gameWorld);

  if (!isConfigured()) {
    return EventProcessStatus::Skipped;
  }

  m_dynamicsWorld->removeRigidBody(event.getComponent().m_backendAdapter->m_rigidBodyInstance);
  event.getComponent().m_backendAdapter = nullptr;

  return EventProcessStatus::Processed;
}

EventProcessStatus BulletPhysicsAdapter::receiveEvent(GameWorld* gameWorld, const GameObjectRemoveEvent& event)
{
  ARG_UNUSED(gameWorld);

  if (!isConfigured()) {
    return EventProcessStatus::Skipped;
  }

  if (event.getObject()->hasComponent<RigidBodyComponent>()) {
    event.getObject()->removeComponent<RigidBodyComponent>();
  }

  return EventProcessStatus::Processed;
}

EventProcessStatus BulletPhysicsAdapter::receiveEvent(GameWorld* gameWorld,
  const GameObjectAddComponentEvent<RigidBodyComponent>& event)
{
  ARG_UNUSED(gameWorld);

  if (!isConfigured()) {
    return EventProcessStatus::Skipped;
  }

  SW_ASSERT(event.getGameObject().hasComponent<TransformComponent>());

  event.getComponent().setTransform(event.getGameObject().getComponent<TransformComponent>().getTransform());

  m_dynamicsWorld->addRigidBody(event.getComponent().m_backendAdapter->m_rigidBodyInstance);

  return EventProcessStatus::Processed;
}

bool BulletPhysicsAdapter::isConfigured() const
{
  return m_dynamicsWorld != nullptr;
}
