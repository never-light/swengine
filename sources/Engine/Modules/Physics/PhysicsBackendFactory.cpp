#include "precompiled.h"

#pragma hdrstop

#include "PhysicsBackendFactory.h"

#include "BulletBackend/BulletPhysicsSystemBackend.h"
#include "BulletBackend/BulletRigidBodyComponent.h"
#include "BulletBackend/BulletKinematicCharacterComponent.h"

std::shared_ptr<PhysicsSystemBackend> PhysicsBackendFactory::createPhysicsSystem(GameWorld* gameWorld)
{
  return std::make_shared<BulletPhysicsSystemBackend>(gameWorld);
}

std::shared_ptr<RigidBodyComponentBackend> PhysicsBackendFactory::createRigidBodyComponent(float mass,
  std::shared_ptr<CollisionShape> collisionShape)
{
  return std::make_shared<BulletRigidBodyComponent>(mass, collisionShape);
}

std::shared_ptr<KinematicCharacterComponentBackend> PhysicsBackendFactory::createKinematicCharacterComponent(
  std::shared_ptr<CollisionShape> collisionShape)
{
  return std::make_shared<BulletKinematicCharacterComponent>(collisionShape);
}
