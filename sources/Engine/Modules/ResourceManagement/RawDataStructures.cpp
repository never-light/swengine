#include "precompiled.h"

#pragma hdrstop

#include <glm/gtc/type_ptr.hpp>
#include "RawDataStructures.h"

RawMatrix4 glmMatrix4ToRawMatrix4(const glm::mat4& matrix) {
  static_assert(sizeof(glm::mat4) == sizeof(RawMatrix4));

  size_t matrixSize = sizeof(matrix);

  RawMatrix4 rawMatrix;
  memcpy_s(reinterpret_cast<void*>(&rawMatrix.data[0]), matrixSize,
           reinterpret_cast<const void*>(glm::value_ptr(matrix)), matrixSize);

  return rawMatrix;
}

glm::mat4 rawMatrix4ToGLMMatrix4(const RawMatrix4& matrix) {
  static_assert(sizeof(glm::mat4) == sizeof(RawMatrix4));

  size_t matrixSize = sizeof(matrix);

  glm::mat4 glmMatrix;
  memcpy_s(reinterpret_cast<void*>(glm::value_ptr(glmMatrix)), matrixSize,
           reinterpret_cast<const void*>(&matrix.data[0]), matrixSize);

  return glmMatrix;
}
