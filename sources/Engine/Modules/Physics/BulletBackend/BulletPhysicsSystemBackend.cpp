#include "precompiled.h"

#pragma hdrstop

#include "BulletPhysicsSystemBackend.h"

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
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
}

void BulletPhysicsSystemBackend::configure()
{
  m_collisionConfiguration = new btDefaultCollisionConfiguration();
  m_collisionDispatcher = new BulletCollisionDispatcher(m_collisionConfiguration, shared_from_this());
  m_broadphaseInterface = new btDbvtBroadphase();
  m_constraintSolver = new btSequentialImpulseConstraintSolver();

  m_collisionDispatcher
    ->setNearCallback(reinterpret_cast<btNearCallback>(BulletPhysicsSystemBackend::physicsNearCallback));

  m_dynamicsWorld = new btDiscreteDynamicsWorld(m_collisionDispatcher,
    m_broadphaseInterface, m_constraintSolver, m_collisionConfiguration);

  m_physicsDebugPainter = new BulletDebugPainter();

  m_gameWorld->subscribeEventsListener<GameObjectAddComponentEvent<RigidBodyComponent>>(this);
  m_gameWorld->subscribeEventsListener<GameObjectRemoveComponentEvent<RigidBodyComponent>>(this);
  m_gameWorld->subscribeEventsListener<GameObjectRemoveEvent>(this);
}

void BulletPhysicsSystemBackend::unconfigure()
{
  m_gameWorld->unsubscribeEventsListener<GameObjectRemoveEvent>(this);
  m_gameWorld->unsubscribeEventsListener<GameObjectRemoveComponentEvent<RigidBodyComponent>>(this);
  m_gameWorld->unsubscribeEventsListener<GameObjectAddComponentEvent<RigidBodyComponent>>(this);

  delete m_physicsDebugPainter;
  m_physicsDebugPainter = nullptr;

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
    dynamic_cast<const BulletRigidBodyComponent*>(&event.getComponent().getBackend());

  m_dynamicsWorld->removeRigidBody(bulletRigidBodyComponent->m_rigidBodyInstance);
  event.getComponent().resetBackend();

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
    dynamic_cast<const BulletRigidBodyComponent*>(&event.getComponent().getBackend());

  bulletRigidBodyComponent->m_rigidBodyInstance->setUserPointer(static_cast<void*>(&event.getGameObject()));

  m_dynamicsWorld->addRigidBody(bulletRigidBodyComponent->m_rigidBodyInstance);

  return EventProcessStatus::Processed;
}

bool BulletPhysicsSystemBackend::isConfigured() const
{
  return m_dynamicsWorld != nullptr;
}

void BulletPhysicsSystemBackend::physicsNearCallback(btBroadphasePair& collisionPair,
  btCollisionDispatcher& dispatcher,
  btDispatcherInfo& dispatchInfo)
{
  dynamic_cast<const BulletCollisionDispatcher&>(dispatcher).getPhysicsSystemBackend().nearCallback(collisionPair,
    dispatcher, dispatchInfo);
}

void BulletPhysicsSystemBackend::nearCallback(btBroadphasePair& collisionPair,
  btCollisionDispatcher& dispatcher, btDispatcherInfo& dispatchInfo)
{
  auto* client1 = static_cast<btCollisionObject*>(collisionPair.m_pProxy0->m_clientObject);
  auto* client2 = static_cast<btCollisionObject*>(collisionPair.m_pProxy1->m_clientObject);

  RigidBodyCollisionProcessingStatus processingStatus = RigidBodyCollisionProcessingStatus::Skipped;

  if (client1->getUserPointer() != nullptr && client2->getUserPointer() != nullptr) {
    auto gameObject1 = static_cast<GameObject*>(client1->getUserPointer());
    auto collisionCallback1 = gameObject1->getComponent<RigidBodyComponent>().getCollisionCallback();

    auto gameObject2 = static_cast<GameObject*>(client2->getUserPointer());
    auto collisionCallback2 = gameObject2->getComponent<RigidBodyComponent>().getCollisionCallback();

    CollisionInfo collisionInfo = { .selfGameObject = *gameObject1, .gameObject = *gameObject2 };

    if (collisionCallback1 != nullptr) {
      processingStatus = collisionCallback1(collisionInfo);
    }

    if (collisionCallback2 != nullptr && processingStatus != RigidBodyCollisionProcessingStatus::Processed) {
      std::swap(collisionInfo.gameObject, collisionInfo.selfGameObject);
      processingStatus = collisionCallback2(collisionInfo);
    }
  }

  if (processingStatus != RigidBodyCollisionProcessingStatus::Processed) {
    btCollisionDispatcher::defaultNearCallback(collisionPair, dispatcher, dispatchInfo);
  }
}

void BulletPhysicsSystemBackend::enableDebugDrawing(bool enable)
{
  m_isDebugDrawingEnabled = enable;

  if (enable) {
    m_dynamicsWorld->setDebugDrawer(m_physicsDebugPainter);
  }
  else {
    m_dynamicsWorld->setDebugDrawer(nullptr);
  }
}

bool BulletPhysicsSystemBackend::isDebugDrawingEnabled()
{
  return m_isDebugDrawingEnabled;
}

void BulletPhysicsSystemBackend::render()
{
  if (isDebugDrawingEnabled()) {
    m_dynamicsWorld->debugDrawWorld();
  }
}
