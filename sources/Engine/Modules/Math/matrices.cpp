#include "precompiled.h"
#pragma hdrstop

#include <cmath>

#include "matrices.h"

bool isMatricesEqual(const glm::mat4& a, const glm::mat4& b, float eps)
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

bool isMatrixIdentity(const glm::mat4& matrix, const float eps)
{
    return isMatricesEqual(matrix, glm::identity<glm::mat4>(), eps);
}

bool isQuatsEqual(const glm::quat& a, const glm::quat& b, float eps)
{
    return glm::abs(glm::dot(a, b)) > 1 - eps;
}

bool isVectorsEqual(const glm::vec3& a, const glm::vec3& b, float eps)
{
    using vector_length_type = glm::vec3::length_type;

    for (vector_length_type coordinateIndex = 0; coordinateIndex < a.length(); coordinateIndex++) {
        if (glm::abs(a[coordinateIndex] - b[coordinateIndex]) > eps) {
            return false;
        }
    }

    return true;
}
