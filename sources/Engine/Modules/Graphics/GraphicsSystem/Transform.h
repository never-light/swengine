#pragma once

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

class Transform {
 public:
  Transform();

  void move(float x, float y, float z);
  void move(const glm::vec3& movement);

  void setPosition(float x, float y, float z);
  void setPosition(const glm::vec3& position);
  [[nodiscard]] glm::vec3 getPosition() const;

  void scale(float x, float y, float z);
  void scale(const glm::vec3& scale);

  void setScale(float x, float y, float z);
  void setScale(const glm::vec3& scale);
  [[nodiscard]] glm::vec3 getScale() const;

  void rotateLocal(float x, float y, float z, float angle);
  void rotateLocal(const glm::vec3& axis, float angle);

  void rotateGlobal(float x, float y, float z, float angle);
  void rotateGlobal(const glm::vec3& axis, float angle);

  void setOrientation(const glm::quat& orientation);
  [[nodiscard]] glm::quat getOrientation() const;

  [[nodiscard]] glm::vec3 getFrontDirection() const;
  [[nodiscard]] glm::vec3 getRightDirection() const;
  [[nodiscard]] glm::vec3 getUpDirection() const;

  void yawLocal(float angle);
  void pitchLocal(float angle);
  void rollLocal(float angle);

  void yawGlobal(float angle);
  void pitchGlobal(float angle);
  void rollGlobal(float angle);

  float getYawValue() const;
  float getPitchValue() const;
  float getRollValue() const;

  void lookAt(float x, float y, float z);
  void lookAt(const glm::vec3& target);

  [[nodiscard]] glm::mat4x4& getTransformationMatrix();

  [[nodiscard]] bool isCacheOutdated() const;

 private:
  void resetTransformationCache();

 private:
  glm::vec3 m_position;
  glm::vec3 m_scale;
  glm::quat m_orientation;

  glm::mat4x4 m_transformationMatrixCache;
  bool m_needTransformationMatrixCacheUpdate = true;
};

