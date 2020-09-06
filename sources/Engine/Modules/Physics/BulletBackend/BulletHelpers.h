#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <btBulletDynamicsCommon.h>

inline btVector3 glmVec3ToBt(const glm::vec3& v) {
  return btVector3(v.x, v.y, v.z);
}

inline glm::vec3 btVec3ToGlm(const btVector3& v) {
  return glm::vec3(v.x(), v.y(), v.z());
}

inline btQuaternion glmQuatToBt(const glm::quat& q) {
  return btQuaternion(q.x, q.y, q.z, q.w);
}

inline glm::quat btQuatToGlm(const btQuaternion& q) {
  return glm::quat(q.w(), q.x(), q.y(), q.z());
}
