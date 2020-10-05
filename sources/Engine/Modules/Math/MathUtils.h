#pragma once

#include <tuple>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "geometry.h"

class MathUtils {
 public:
  static bool isEqual(const glm::mat4& a, const glm::mat4& b, float eps = 1e-6f);
  static bool isEqual(const glm::quat& a, const glm::quat& b, float eps = 1e-6f);
  static bool isEqual(const glm::vec3& a, const glm::vec3& b, float eps = 1e-6f);
  static bool isEqual(float a, float b, float eps = 1e-6f);
  static bool isEqual(const Plane& a, const Plane& b);

  static bool isZero(float a, float eps = 1e-6f);

  static bool isMatrixIdentity(const glm::mat4& matrix, float eps = 1e-6f);

  static glm::mat4 getTranslationMatrix(const glm::vec3& translation);
  static glm::mat4 getRotationMatrix(const glm::vec3& axis, float angle);
  static glm::mat4 getRotationMatrixFromYawPitchRoll(float yaw, float pitch, float roll);
  static glm::mat4 getScaleMatrix(const glm::vec3& scale);

  static glm::mat4 getLookAtViewMatrix(const glm::vec3& viewerPosition, const glm::vec3& targetPosition,
    const glm::vec3& upAxis = AXIS_Y);

  static glm::mat4 getPerspectiveProjectionMatrix(float fovY,
    float aspectRatio,
    float nearDistance,
    float farDistance);

  static glm::mat4 getYawMatrix(float angle);
  static glm::mat4 getPitchMatrix(float angle);
  static glm::mat4 getRollMatrix(float angle);

  static std::tuple<glm::vec3, glm::vec3> quatToForwardUp(const glm::quat& q);
  static glm::quat forwardUpToQuat(const glm::vec3& forward, const glm::vec3& up);

  static float fractionToPercents(float numerator, float denominator);
  static float fractionToPercents(int numerator, int denominator);

 public:
  static constexpr glm::vec3 AXIS_X = {1.0f, 0.0f, 0.0f};
  static constexpr glm::vec3 AXIS_Y = {0.0f, 1.0f, 0.0f};
  static constexpr glm::vec3 AXIS_Z = {0.0f, 0.0f, 1.0f};

  static constexpr glm::vec3 AXIS_FORWARD = -AXIS_Z;
  static constexpr glm::vec3 AXIS_UP = AXIS_Y;
  static constexpr glm::vec3 AXIS_RIGHT = AXIS_X;

  static constexpr glm::mat4 IDENTITY_MATRIX4 = glm::identity<glm::mat4>();
  static constexpr glm::quat IDENTITY_QUAT = glm::identity<glm::quat>();

  static constexpr glm::vec3 ZERO_VEC3 = {0.0f, 0.0f, 0.0f};
};

template<class T>
bool isVectorsEqual(const T& a, const T& b, float eps = 1e-6f)
{
  using vector_length_type = typename T::length_type;

  for (vector_length_type coordinateIndex = 0; coordinateIndex < a.length(); coordinateIndex++) {
    if (glm::abs(a[coordinateIndex] - b[coordinateIndex]) > eps) {
      return false;
    }
  }

  return true;
}
