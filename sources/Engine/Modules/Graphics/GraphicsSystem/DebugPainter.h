#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class DebugPainter
{
public:
    DebugPainter();

    void renderSegment(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
    void renderVector(const glm::vec3& start, const glm::vec3& end, const glm::vec4& color);
    void renderBasis(const glm::vec3& x, const glm::vec3& y, const glm::vec3& z);

    void renderSphere(const glm::vec3& centerPosition, float radius, const glm::vec4& color = {}, bool wireframe = true);
    void renderBox(const glm::vec3& centerPosition, const glm::vec3& halfSize, const glm::vec4& color = {}, bool wireframe = true);

    void renderWorldGrid(const glm::vec3& drawOrigin, const glm::vec4& color);
};

