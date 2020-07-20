#pragma once

#include <memory>
#include "CollisionShapes.h"
#include "BulletBackend/BulletCollisionShapesAdapter.h"

class CollisionShapesFactory {
 public:
  static std::shared_ptr<CollisionShapeSphere> createSphere(float radius) {
    return std::make_shared<BulletCollisionShapeSphere>(radius);
  }

  static std::shared_ptr<CollisionShapeBox> createBox(const glm::vec3& halfExtents) {
    return std::make_shared<BulletCollisionShapeBox>(halfExtents);
  }

  static std::shared_ptr<CollisionShapeCapsule> createCapsule(float radius, float height) {
    return std::make_shared<BulletCollisionShapeCapsule>(radius, height);
  }
};
