#include "precompiled.h"
#pragma hdrstop

#include <cmath>

#include "matrices.h"

bool isMatrixIdentity(const glm::mat4& matrix, const float eps)
{
    for (size_t c = 0; c < 4; c++) {
        for (size_t r = 0; r < 4; r++) {
            if (r == c && std::fabs(matrix[c][r] - 1) > eps) {
                return false;
            }

            if (r != c && std::fabs(matrix[c][r]) > eps) {
                return false;
            }
        }
    }

    return true;
}
