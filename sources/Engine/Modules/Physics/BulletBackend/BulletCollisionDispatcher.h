#pragma once

#include <memory>
#include <btBulletCollisionCommon.h>

class BulletPhysicsSystemBackend;

class BulletCollisionDispatcher : public btCollisionDispatcher {
 public:
  BulletCollisionDispatcher(btCollisionConfiguration* collisionConfiguration,
    std::weak_ptr<BulletPhysicsSystemBackend> physicsSystemBackend);

  ~BulletCollisionDispatcher() override = default;

  [[nodiscard]] BulletPhysicsSystemBackend& getPhysicsSystemBackend() const;

 private:
  std::weak_ptr<BulletPhysicsSystemBackend> m_physicsSystemBackend;
};