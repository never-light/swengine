#pragma once

#include <glm/vec2.hpp>

class Rect {
 public:
  Rect();
  Rect(const Rect& rect);
  Rect(const glm::ivec2& origin, const glm::ivec2& size);
  Rect(int originX, int originY, int width, int height);

  bool isPointInRect(const glm::ivec2& point) const;

  int getWidth() const;
  int getHeight() const;
  const glm::ivec2& getSize() const;

  int getOriginX() const;
  int getOriginY() const;
  const glm::ivec2& getOrigin() const;

 private:
  glm::ivec2 m_origin;
  glm::ivec2 m_size;
};

