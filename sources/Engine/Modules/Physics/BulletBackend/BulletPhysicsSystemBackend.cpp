#include "precompiled.h"

#pragma hdrstop

#include "BulletPhysicsSystemBackend.h"

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Physics/RigidBodyComponent.h"

#include "BulletRigidBodyComponent.h"

BulletPhysicsSystemBackend::BulletPhysicsSystemBackend(std::shared_ptr<GameWorld> gameWorld)
  : m_gameWorld(gameWorld)
{
}

BulletPhysicsSystemBackend::~BulletPhysicsSystemBackend()
{
  SW_ASSERT(m_dynamicsWorld == nullptr &&
    m_constraintSolver == nullptr &&
    m_broadphaseInterface == nullptr &&
    m_collisionDispatcher == nullptr &&
    m_collisionConfiguration == nullptr);
}

glm::vec3 BulletPhysicsSystemBackend::getGravity() const
{
  btVector3 gravity = m_dynamicsWorld->getGravity();

  return {gravity.x(), gravity.y(), gravity.z()};
}

void BulletPhysicsSystemBackend::setGravity(const glm::vec3& gravity)
{
  m_dynamicsWorld->setGravity({gravity.x, gravity.y, gravity.z});
}

void BulletPhysicsSystemBackend::update(float delta)
{
  m_dynamicsWorld->stepSimulation(delta, 60);

  // Update game objects transforms after the simulation
  // TODO: override Bullet Motion State and move synchronization logic into the inheritor

  //  for (auto gameObject : m_gameWorld->allWith<RigidBodyComponent>()) {
  //    auto& transform = gameObject->getComponent<TransformComponent>().getTransform();
  //    gameObject->getComponent<RigidBodyComponent>().requestTransform(transform);
  //  }
}

void BulletPhysicsSystemBackend::configure()
{
  m_collisionConfiguration = new btDefaultCollisionConfiguration();
  m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);
  m_broadphaseInterface = new btDbvtBroadphase();
  m_constraintSolver = new btSequentialImpulseConstraintSolver();

  m_dynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher,
    m_broadphaseInterface, m_constraintSolver, m_collisionConfiguration);

  m_gameWorld->subscribeEventsListener<GameObjectAddComponentEvent<RigidBodyComponent>>(this);
  m_gameWorld->subscribeEventsListener<GameObjectRemoveComponentEvent<RigidBodyComponent>>(this);
  m_gameWorld->subscribeEventsListener<GameObjectRemoveEvent>(this);
}

void BulletPhysicsSystemBackend::unconfigure()
{
  m_gameWorld->unsubscribeEventsListener<GameObjectRemoveEvent>(this);
  m_gameWorld->unsubscribeEventsListener<GameObjectRemoveComponentEvent<RigidBodyComponent>>(this);
  m_gameWorld->unsubscribeEventsListener<GameObjectAddComponentEvent<RigidBodyComponent>>(this);

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

EventProcessStatus BulletPhysicsSystemBackend::receiveEvent(GameWorld* gameWorld,
  const GameObjectRemoveComponentEvent<RigidBodyComponent>& event)
{
  ARG_UNUSED(gameWorld);

  if (!isConfigured()) {
    return EventProcessStatus::Skipped;
  }

  const auto* bulletRigidBodyComponent =
    dynamic_cast<const BulletRigidBodyComponent*>(event.getComponent().m_backendAdapter.get());

  m_dynamicsWorld->removeRigidBody(bulletRigidBodyComponent->m_rigidBodyInstance);
  event.getComponent().m_backendAdapter = nullptr;

  return EventProcessStatus::Processed;
}

EventProcessStatus BulletPhysicsSystemBackend::receiveEvent(GameWorld* gameWorld, const GameObjectRemoveEvent& event)
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

EventProcessStatus BulletPhysicsSystemBackend::receiveEvent(GameWorld* gameWorld,
  const GameObjectAddComponentEvent<RigidBodyComponent>& event)
{
  ARG_UNUSED(gameWorld);

  if (!isConfigured()) {
    return EventProcessStatus::Skipped;
  }

  SW_ASSERT(event.getGameObject().hasComponent<TransformComponent>());

  event.getComponent().setTransform(event.getGameObject().getComponent<TransformComponent>().getTransform());

  const auto* bulletRigidBodyComponent =
    dynamic_cast<const BulletRigidBodyComponent*>(event.getComponent().m_backendAdapter.get());

  m_dynamicsWorld->addRigidBody(bulletRigidBodyComponent->m_rigidBodyInstance);

  return EventProcessStatus::Processed;
}

bool BulletPhysicsSystemBackend::isConfigured() const
{
  return m_dynamicsWorld != nullptr;
}
