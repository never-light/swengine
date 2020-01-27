#pragma once

#include <glm/mat4x4.hpp>

bool isMatrixIdentity(const glm::mat4& matrix, const float eps = 1e-6f);
