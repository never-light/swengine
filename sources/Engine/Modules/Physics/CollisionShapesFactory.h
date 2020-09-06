#pragma once

#include <memory>
#include "CollisionShapes.h"

class CollisionShapesFactory {
 public:
  static std::shared_ptr<CollisionShapeSphere> createSphere(float radius)
  {
    return std::make_shared<CollisionShapeSphere>(radius);
  }

  static std::shared_ptr<CollisionShapeBox> createBox(const glm::vec3& halfExtents)
  {
    return std::make_shared<CollisionShapeBox>(halfExtents);
  }

  static std::shared_ptr<CollisionShapeCapsule> createCapsule(float radius, float height)
  {
    return std::make_shared<CollisionShapeCapsule>(radius, height);
  }

  static std::shared_ptr<CollisionShapeCompound> createCompoundShape(
    const std::vector<CollisionShapeCompoundChild>& children)
  {
    return std::make_shared<CollisionShapeCompound>(children);
  }

};
