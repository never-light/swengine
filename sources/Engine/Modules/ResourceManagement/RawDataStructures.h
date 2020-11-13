#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

#include "types.h"

struct RawVector2 {
  float x;
  float y;
};

struct RawVector3 {
  float x;
  float y;
  float z;
};

struct RawVector4 {
  float x;
  float y;
  float z;
  float w;
};

struct RawU8Vector4 {
  union {
    struct {
      uint8_t x;
      uint8_t y;
      uint8_t z;
      uint8_t w;
    };

    uint8_t data[4];
  };
};

struct RawMatrix3 {
  float data[12];
};

struct RawMatrix4 {
  float data[16];
};

struct RawQuaternion {
  float x;
  float y;
  float z;
  float w;
};

RawMatrix4 glmMatrix4ToRawMatrix4(const glm::mat4& matrix);
glm::mat4 rawMatrix4ToGLMMatrix4(const RawMatrix4& matrix);
