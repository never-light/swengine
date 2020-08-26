#include "precompiled.h"

#pragma hdrstop

#include "BulletCollisionDispatcher.h"

#include <utility>

BulletCollisionDispatcher::BulletCollisionDispatcher(btCollisionConfiguration* collisionConfiguration,
  std::weak_ptr<BulletPhysicsSystemBackend> physicsSystemBackend)
    : btCollisionDispatcher(collisionConfiguration),
    m_physicsSystemBackend(std::move(physicsSystemBackend))
{

}

BulletPhysicsSystemBackend& BulletCollisionDispatcher::getPhysicsSystemBackend() const
{
  return *m_physicsSystemBackend.lock();
}
