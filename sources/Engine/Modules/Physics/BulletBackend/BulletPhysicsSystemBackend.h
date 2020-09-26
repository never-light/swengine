#pragma once

#include <glm/vec3.hpp>
#include <btBulletDynamicsCommon.h>

#include "Modules/ECS/ECS.h"

#include "Modules/Physics/BaseBackend/PhysicsSystemBackend.h"
#include "Modules/Physics/RigidBodyComponent.h"
#include "Modules/Physics/KinematicCharacterComponent.h"
#include "Modules/Physics/PhysicsCollisions.h"

#include "BulletCollisionDispatcher.h"
#include "BulletDebugPainter.h"
#include "BulletKinematicCharacterComponent.h"

// TODO: fix possible circular dependency here, replace shared_ptr to GameWorld with weak_ptr

class BulletPhysicsSystemBackend :
  public PhysicsSystemBackend,
  public std::enable_shared_from_this<BulletPhysicsSystemBackend>,
  public EventsListener<GameObjectAddComponentEvent<RigidBodyComponent>>,
  public EventsListener<GameObjectRemoveComponentEvent<RigidBodyComponent>>,
  public EventsListener<GameObjectAddComponentEvent<KinematicCharacterComponent>>,
  public EventsListener<GameObjectRemoveComponentEvent<KinematicCharacterComponent>> {
 public:
  explicit BulletPhysicsSystemBackend(GameWorld* gameWorld);
  ~BulletPhysicsSystemBackend() override;

  void configure() override;
  void unconfigure() override;

  void setGravity(const glm::vec3& gravity) override;
  glm::vec3 getGravity() const override;

  void update(float delta) override;
  void render() override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld,
    const GameObjectAddComponentEvent<RigidBodyComponent>& event) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld,
    const GameObjectRemoveComponentEvent<RigidBodyComponent>& event) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld,
    const GameObjectAddComponentEvent<KinematicCharacterComponent>& event) override;

  EventProcessStatus receiveEvent(GameWorld* gameWorld,
    const GameObjectRemoveComponentEvent<KinematicCharacterComponent>& event) override;

  void enableDebugDrawing(bool enable) override;
  bool isDebugDrawingEnabled() override;

  void setUpdateStepCallback(std::function<void(float)> callback) override;

 private:
  bool isConfigured() const;

  void nearCallback(btBroadphasePair& collisionPair,
    btCollisionDispatcher& dispatcher, btDispatcherInfo& dispatchInfo);
  static CollisionCallback getCollisionsCallback(GameObject& object) ;

  static void synchronizeTransforms(GameObject& object, const btTransform& transform);

 private:
  static void physicsNearCallback(btBroadphasePair& collisionPair,
    btCollisionDispatcher& dispatcher, btDispatcherInfo& dispatchInfo);

  static void physicsTickCallback(btDynamicsWorld* world, btScalar timeStep);

 private:
  GameWorld* m_gameWorld;

  btDefaultCollisionConfiguration* m_collisionConfiguration = nullptr;
  BulletCollisionDispatcher* m_collisionDispatcher = nullptr;
  btBroadphaseInterface* m_broadphaseInterface = nullptr;
  btSequentialImpulseConstraintSolver* m_constraintSolver = nullptr;
  btDiscreteDynamicsWorld* m_dynamicsWorld = nullptr;
  BulletDebugPainter* m_physicsDebugPainter = nullptr;

  bool m_isDebugDrawingEnabled = false;

  std::function<void(float)> m_updateStepCallback;
};
