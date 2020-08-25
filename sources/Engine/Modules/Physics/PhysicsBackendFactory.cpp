#include "precompiled.h"

#pragma hdrstop

#include "PhysicsBackendFactory.h"

#include "BulletBackend/BulletPhysicsSystemBackend.h"
#include "BulletBackend/BulletRigidBodyComponent.h"

std::unique_ptr<PhysicsSystemBackend> PhysicsBackendFactory::createPhysicsSystem(std::shared_ptr<GameWorld> gameWorld)
{
  return std::make_unique<BulletPhysicsSystemBackend>(gameWorld);
}

std::shared_ptr<RigidBodyComponentBackend> PhysicsBackendFactory::createRigidBodyComponent(float mass,
  std::shared_ptr<CollisionShape> collisionShape,
  std::shared_ptr<Transform> gameTransform)
{
  return std::make_shared<BulletRigidBodyComponent>(mass, collisionShape, gameTransform);
}
