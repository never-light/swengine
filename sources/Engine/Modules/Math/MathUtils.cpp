#include "MathUtils.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

glm::mat4 MathUtils::getTranslationMatrix(const glm::vec3& translation)
{
  return glm::translate(IDENTITY_MATRIX4, translation);
}

glm::mat4 MathUtils::getRotationMatrix(const glm::vec3& axis, float angle)
{
  return glm::rotate(IDENTITY_MATRIX4, glm::radians(angle), axis);
}

glm::mat4 MathUtils::getRotationMatrixFromYawPitchRoll(float yaw, float pitch, float roll)
{
  return glm::yawPitchRoll(glm::radians(yaw), glm::radians(pitch), glm::radians(roll));
}

glm::mat4 MathUtils::getScaleMatrix(const glm::vec3& scale)
{
  return glm::scale(IDENTITY_MATRIX4, scale);
}

glm::mat4 MathUtils::getYawMatrix(float angle)
{
  return getRotationMatrix(AXIS_Y, angle);
}

glm::mat4 MathUtils::getPitchMatrix(float angle)
{
  return getRotationMatrix(AXIS_X, angle);
}

glm::mat4 MathUtils::getRollMatrix(float angle)
{
  return getRotationMatrix(AXIS_Z, angle);
}

bool MathUtils::isEqual(const glm::mat4& a, const glm::mat4& b, float eps)
{
  using matrix_length_type = glm::mat4::length_type;

  for (matrix_length_type c = 0; c < 4; c++) {
    for (matrix_length_type r = 0; r < 4; r++) {
      if (glm::abs(a[c][r] - b[c][r]) > eps) {
        return false;
      }
    }
  }

  return true;
}

bool MathUtils::isEqual(const glm::quat& a, const glm::quat& b, float eps)
{
  return glm::abs(glm::dot(a, b)) > 1 - eps;
}

bool MathUtils::isEqual(const glm::vec3& a, const glm::vec3& b, float eps)
{
  return isVectorsEqual(a, b, eps);
}

bool MathUtils::isEqual(const glm::vec4& a, const glm::vec4& b, float eps)
{
  return isVectorsEqual(a, b, eps);
}

bool MathUtils::isEqual(float a, float b, float eps)
{
  return glm::abs(a - b) <= eps;
}

bool MathUtils::isEqual(const Plane& a, const Plane& b)
{
  return isEqual(a.getNormal(), b.getNormal()) && isEqual(a.getDistance(), b.getDistance());
}

bool MathUtils::isMatrixIdentity(const glm::mat4& matrix, float eps)
{
  return isEqual(matrix, glm::identity<glm::mat4>(), eps);
}

std::tuple<glm::vec3, glm::vec3> MathUtils::quatToForwardUp(const glm::quat& q)
{
  return {q * AXIS_FORWARD, q * AXIS_UP};
}

glm::quat MathUtils::forwardUpToQuat(const glm::vec3& forward, const glm::vec3& up)
{
  return glm::quatLookAt(forward, up);
}

bool MathUtils::isZero(float a, float eps)
{
  return glm::abs(a) <= eps;
}

float MathUtils::fractionToPercents(float numerator, float denominator)
{
  return (numerator / denominator) * 100.0f;
}

float MathUtils::fractionToPercents(int numerator, int denominator)
{
  return (static_cast<float>(numerator) / static_cast<float>(denominator)) * 100.0f;
}

glm::mat4 MathUtils::getLookAtViewMatrix(
  const glm::vec3& viewerPosition,
  const glm::vec3& targetPosition,
  const glm::vec3& upAxis)
{
  return glm::lookAt(viewerPosition, targetPosition, upAxis);
}

glm::mat4 MathUtils::getPerspectiveProjectionMatrix(float fovY,
  float aspectRatio,
  float nearDistance,
  float farDistance)
{
  return glm::perspective(fovY, aspectRatio, nearDistance, farDistance);
}

glm::mat4 MathUtils::getRotationMatrix(const glm::quat& orientation)
{
  return glm::mat4_cast(orientation);
}

std::tuple<glm::vec3, glm::vec3> MathUtils::generateTangentBitangent(const glm::vec3& v0,
  const glm::vec3& v1,
  const glm::vec3& v2,
  const glm::vec2& uv0,
  const glm::vec2& uv1,
  const glm::vec2& uv2)
{
  glm::vec3 positionDifference1 = v1 - v0;
  glm::vec3 positionDifference2 = v2 - v0;

  glm::vec2 uvDifference1 = uv1 - uv0;
  glm::vec2 uvDifference2 = uv2 - uv0;

  float r = 1.0f / (uvDifference1.x * uvDifference2.y - uvDifference1.y * uvDifference2.x);
  glm::vec3 tangent = (positionDifference1 * uvDifference2.y - positionDifference2 * uvDifference1.y) * r;
  glm::vec3 bitangent = (positionDifference2 * uvDifference1.x - positionDifference1 * uvDifference2.x) * r;

  return {tangent, bitangent};
}

glm::vec3 MathUtils::generateTangent(const glm::vec3& v0,
  const glm::vec3& v1,
  const glm::vec3& v2,
  const glm::vec2& uv0,
  const glm::vec2& uv1,
  const glm::vec2& uv2)
{
  return std::get<0>(generateTangentBitangent(v0, v1, v2, uv0, uv1, uv2));
}

glm::vec3 MathUtils::extractScale2(const glm::mat4& matrix)
{
  return glm::vec3(glm::length2(glm::vec3(matrix[0])),
    glm::length2(glm::vec3(matrix[1])),
    glm::length2(glm::vec3(matrix[2])));
}
