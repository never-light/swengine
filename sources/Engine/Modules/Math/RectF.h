#pragma once

#include <glm/vec2.hpp>

class RectF {
 public:
  RectF();
  RectF(const RectF& rect);
  RectF(const glm::vec2& origin, const glm::vec2& size);

  [[nodiscard]] float getWidth() const;
  [[nodiscard]] float getHeight() const;
  [[nodiscard]] const glm::vec2& getSize() const;

  [[nodiscard]] float getOriginX() const;
  [[nodiscard]] float getOriginY() const;
  [[nodiscard]] const glm::vec2& getOrigin() const;

 private:
  glm::vec2 m_origin;
  glm::vec2 m_size;
};

