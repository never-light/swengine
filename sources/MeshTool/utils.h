#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <Engine/Modules/ResourceManagement/RawDataStructures.h>

inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from) {
  glm::mat4 to;

  to[0][0] = static_cast<float>(from->a1);
  to[0][1] = static_cast<float>(from->b1);
  to[0][2] = static_cast<float>(from->c1);
  to[0][3] = static_cast<float>(from->d1);

  to[1][0] = static_cast<float>(from->a2);
  to[1][1] = static_cast<float>(from->b2);
  to[1][2] = static_cast<float>(from->c2);
  to[1][3] = static_cast<float>(from->d2);

  to[2][0] = static_cast<float>(from->a3);
  to[2][1] = static_cast<float>(from->b3);
  to[2][2] = static_cast<float>(from->c3);
  to[2][3] = static_cast<float>(from->d3);

  to[3][0] = static_cast<float>(from->a4);
  to[3][1] = static_cast<float>(from->b4);
  to[3][2] = static_cast<float>(from->c4);
  to[3][3] = static_cast<float>(from->d4);

  return to;
}

inline glm::vec3 aiVec3ToGlm(const aiVector3D& vector) {
  return glm::vec3(vector.x, vector.y, vector.z);
}

inline glm::quat aiQuaternionToGlm(const aiQuaternion& from) {
  glm::quat to;
  to.x = from.x;
  to.y = from.y;
  to.z = from.z;
  to.w = from.w;
  return to;
}

inline RawVector3 getTransformedRawVector(const aiVector3D& vector,
                                          const aiMatrix4x4& matrix,
                                          bool point = true,
                                          bool normalize = false) {
  glm::vec4 sourceVector = glm::vec4(aiVec3ToGlm(vector), point ? 1 : 0);
  glm::vec3 transformedVector = aiMatrix4x4ToGlm(&matrix) * sourceVector;

  if (normalize) {
    transformedVector = glm::normalize(transformedVector);
  }

  return {transformedVector.x, transformedVector.y, transformedVector.z};
}

inline RawVector3 rawVectorMin(const RawVector3& a, const RawVector3& b) {
  RawVector3 result;
  result.x = std::fminf(a.x, b.x);
  result.y = std::fminf(a.y, b.y);
  result.z = std::fminf(a.z, b.z);

  return result;
}
