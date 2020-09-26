#include "precompiled.h"

#pragma hdrstop

#include "BulletPhysicsSystemBackend.h"

#include <utility>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "Modules/Graphics/GraphicsSystem/TransformComponent.h"
#include "Modules/Graphics/GraphicsSystem/MeshRendererComponent.h"

#include "BulletRigidBodyComponent.h"
#include "BulletUtils.h"

BulletPhysicsSystemBackend::BulletPhysicsSystemBackend(GameWorld* gameWorld)
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

  m_broadphaseInterface->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

  m_physicsDebugPainter = new BulletDebugPainter();

  m_gameWorld->subscribeEventsListener<GameObjectAddComponentEvent<RigidBodyComponent>>(this);
  m_gameWorld->subscribeEventsListener<GameObjectRemoveComponentEvent<RigidBodyComponent>>(this);

  m_gameWorld->subscribeEventsListener<GameObjectAddComponentEvent<KinematicCharacterComponent>>(this);
  m_gameWorld->subscribeEventsListener<GameObjectRemoveComponentEvent<KinematicCharacterComponent>>(this);
}

void BulletPhysicsSystemBackend::unconfigure()
{
  m_gameWorld->unsubscribeEventsListener<GameObjectRemoveComponentEvent<KinematicCharacterComponent>>(this);
  m_gameWorld->unsubscribeEventsListener<GameObjectAddComponentEvent<KinematicCharacterComponent>>(this);

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
    dynamic_cast<const BulletRigidBodyComponent*>(&event.component->getBackend());

  m_dynamicsWorld->removeRigidBody(bulletRigidBodyComponent->m_rigidBodyInstance);
  event.component->resetBackend();

  return EventProcessStatus::Processed;
}

EventProcessStatus BulletPhysicsSystemBackend::receiveEvent(GameWorld* gameWorld,
  const GameObjectAddComponentEvent<RigidBodyComponent>& event)
{
  ARG_UNUSED(gameWorld);

  if (!isConfigured()) {
    return EventProcessStatus::Skipped;
  }

  SW_ASSERT(event.gameObject.hasComponent<TransformComponent>());

  GameObjectComponentHandle<RigidBodyComponent> rigidBodyComponent = event.component;

  rigidBodyComponent->setTransform(event.gameObject.getComponent<TransformComponent>()->getTransform());

  GameObjectId gameObjectId = event.gameObject.getId();

  const auto* bulletRigidBodyComponent =
    dynamic_cast<const BulletRigidBodyComponent*>(&rigidBodyComponent->getBackend());

  btRigidBody* rigidBodyInstance = bulletRigidBodyComponent->m_rigidBodyInstance;
  rigidBodyInstance->setUserPointer(reinterpret_cast<void*>(static_cast<uintptr_t>(gameObjectId)));

  auto* motionState = dynamic_cast<BulletMotionState*>(rigidBodyInstance->getMotionState());

  motionState->setUpdateCallback([gameObjectId, this](const btTransform& transform) {
    auto obj = this->m_gameWorld->findGameObject(gameObjectId);
    SW_ASSERT(obj.isAlive());

    BulletPhysicsSystemBackend::synchronizeTransforms(obj, transform);
  });

  m_dynamicsWorld->addRigidBody(bulletRigidBodyComponent->m_rigidBodyInstance);

  return EventProcessStatus::Processed;
}

EventProcessStatus BulletPhysicsSystemBackend::receiveEvent(GameWorld* gameWorld,
  const GameObjectRemoveComponentEvent<KinematicCharacterComponent>& event)
{
  ARG_UNUSED(gameWorld);

  if (!isConfigured()) {
    return EventProcessStatus::Skipped;
  }

  GameObjectComponentHandle<KinematicCharacterComponent> kinematicCharacterComponent = event.component;

  const auto* bulletKinematicComponent =
    dynamic_cast<const BulletKinematicCharacterComponent*>(&kinematicCharacterComponent->getBackend());

  m_dynamicsWorld->removeCollisionObject(bulletKinematicComponent->m_ghostObject);
  m_dynamicsWorld->removeAction(bulletKinematicComponent->m_kinematicController);

  kinematicCharacterComponent->resetBackend();

  return EventProcessStatus::Processed;
}

EventProcessStatus BulletPhysicsSystemBackend::receiveEvent(GameWorld* gameWorld,
  const GameObjectAddComponentEvent<KinematicCharacterComponent>& event)
{
  ARG_UNUSED(gameWorld);

  if (!isConfigured()) {
    return EventProcessStatus::Skipped;
  }

  SW_ASSERT(event.gameObject.hasComponent<TransformComponent>());

  GameObjectComponentHandle<KinematicCharacterComponent> kinematicCharacterComponent = event.component;

  kinematicCharacterComponent->setTransform(event.gameObject.getComponent<TransformComponent>()->getTransform());

  GameObjectId gameObjectId = event.gameObject.getId();

  auto* bulletKinematicComponent =
    dynamic_cast<BulletKinematicCharacterComponent*>(&kinematicCharacterComponent->getBackend());

  btGhostObject* ghostObject = bulletKinematicComponent->m_ghostObject;
  ghostObject->setUserPointer(reinterpret_cast<void*>(static_cast<uintptr_t>(gameObjectId)));

  auto* kinematicController = bulletKinematicComponent->m_kinematicController;

  kinematicController->setGravity(m_dynamicsWorld->getGravity());

  bulletKinematicComponent->setUpdateCallback([gameObjectId, this](const btTransform& transform) {
    GameObject object = this->m_gameWorld->findGameObject(gameObjectId);
    BulletPhysicsSystemBackend::synchronizeTransforms(object, transform);
  });

  m_dynamicsWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
  m_dynamicsWorld->addAction(kinematicController);

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
    auto clientId1 = static_cast<GameObjectId>(reinterpret_cast<uintptr_t>(client1->getUserPointer()));
    auto clientId2 = static_cast<GameObjectId>(reinterpret_cast<uintptr_t>(client2->getUserPointer()));

    auto gameObject1 = m_gameWorld->findGameObject(clientId1);
    auto collisionCallback1 = getCollisionsCallback(gameObject1);

    auto gameObject2 = m_gameWorld->findGameObject(clientId2);
    auto collisionCallback2 = getCollisionsCallback(gameObject2);

    CollisionInfo collisionInfo = {.selfGameObject = gameObject1, .gameObject = gameObject2};

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

CollisionCallback BulletPhysicsSystemBackend::getCollisionsCallback(GameObject& object)
{
  if (object.hasComponent<RigidBodyComponent>()) {
    return object.getComponent<RigidBodyComponent>()->getCollisionCallback();
  }
  else if (object.hasComponent<KinematicCharacterComponent>()) {
    return object.getComponent<KinematicCharacterComponent>()->getCollisionCallback();
  }
  else {
    return nullptr;
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

void BulletPhysicsSystemBackend::physicsTickCallback(btDynamicsWorld* world, btScalar timeStep)
{
  auto* physicsBackend = reinterpret_cast<BulletPhysicsSystemBackend*>(world->getWorldUserInfo());

  physicsBackend->m_updateStepCallback(static_cast<float>(timeStep));
}

void BulletPhysicsSystemBackend::setUpdateStepCallback(std::function<void(float)> callback)
{
  m_updateStepCallback = callback;

  if (m_updateStepCallback) {
    m_dynamicsWorld->setInternalTickCallback(BulletPhysicsSystemBackend::physicsTickCallback,
      reinterpret_cast<void*>(this), true);
  }
  else {
    m_dynamicsWorld->setInternalTickCallback(nullptr,
      reinterpret_cast<void*>(this), true);
  }
}

void BulletPhysicsSystemBackend::synchronizeTransforms(GameObject& object, const btTransform& transform)
{
  glm::quat orientation = BulletUtils::btQuatToGlm(transform.getRotation());
  glm::vec3 origin = BulletUtils::btVec3ToGlm(transform.getOrigin());

  if (object.hasComponent<TransformComponent>()) {
    auto& objectTransformComponent = *object.getComponent<TransformComponent>().get();
    auto& objectTransform = objectTransformComponent.getTransform();

    objectTransform.setOrientation(orientation);
    objectTransform.setPosition(origin);

    objectTransformComponent.updateBounds(origin, orientation);
  }
}
