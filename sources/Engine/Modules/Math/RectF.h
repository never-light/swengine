#pragma once

#include <glm/vec2.hpp>

class RectF {
public:
    RectF();
    RectF(const RectF& rect);
    RectF(const glm::vec2& origin, const glm::vec2& size);
    RectF(float originX, float originY, float width, float height);

    float getWidth() const;
    float getHeight() const;
    const glm::vec2& getSize() const;

    float getOriginX() const;
    float getOriginY() const;
    const glm::vec2& getOrigin() const;

private:
    glm::vec2 m_origin;
    glm::vec2 m_size;
};

