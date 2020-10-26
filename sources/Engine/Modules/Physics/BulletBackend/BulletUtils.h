#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <btBulletDynamicsCommon.h>

#include "Modules/Graphics/GraphicsSystem/Transform.h"
#include "Modules/Physics/CollisionShapes.h"

class BulletUtils {
 public:
  static inline btVector3 glmVec3ToBt(const glm::vec3& v)
  {
    return btVector3(v.x, v.y, v.z);
  }

  static inline glm::vec3 btVec3ToGlm(const btVector3& v)
  {
    return glm::vec3(v.x(), v.y(), v.z());
  }

  static inline btQuaternion glmQuatToBt(const glm::quat& q)
  {
    return btQuaternion(q.x, q.y, q.z, q.w);
  }

  static inline glm::quat btQuatToGlm(const btQuaternion& q)
  {
    return glm::quat(q.w(), q.x(), q.y(), q.z());
  }

  static inline btTransform internalTransformToBt(const Transform& transform) {
    btTransform bulletTransform;
    bulletTransform.setIdentity();

    bulletTransform.setOrigin(BulletUtils::glmVec3ToBt(transform.getPosition()));
    bulletTransform.setRotation(BulletUtils::glmQuatToBt(transform.getOrientation()));

    return bulletTransform;
  }

  static btCollisionShape* convertCollisionShapeToBulletShape(const CollisionShape& shape);
  static btCollisionShape* convertCollisionShapeToBulletShape(const CollisionShapeData& shape);
};

