#pragma once

#include <glm/vec3.hpp>
#include <btBulletDynamicsCommon.h>

inline btVector3 glmVec3ToBt(const glm::vec3& v) {
  return btVector3(v.x, v.y, v.z);
}

inline glm::vec3 btVec3ToGlm(const btVector3& v) {
  return glm::vec3(v.x(), v.y(), v.z());
}
