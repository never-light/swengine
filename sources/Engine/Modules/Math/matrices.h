#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

bool isMatricesEqual(const glm::mat4& a, const glm::mat4& b, float eps = 1e-6f);
bool isMatrixIdentity(const glm::mat4& matrix, const float eps = 1e-6f);

bool isQuatsEqual(const glm::quat& a, const glm::quat& b, float eps = 1e-6f);
