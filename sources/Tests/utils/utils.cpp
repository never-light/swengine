#include "utils.h"

#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <Engine/Modules/Math/matrices.h>

namespace tests {

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
  return isMatricesEqual(a, b, eps);
}

bool MathUtils::isEqual(const glm::quat& a, const glm::quat& b, float eps)
{
  return isQuatsEqual(a, b, eps);
}

bool MathUtils::isEqual(const glm::vec3& a, const glm::vec3& b, float eps)
{
  return isVectorsEqual(a, b, eps);
}

bool MathUtils::isEqual(float a, float b, float eps)
{
  return glm::abs(a - b) <= eps;
}

}
