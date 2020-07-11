#pragma once

#include <type_traits>
#include <glm/vec2.hpp>

template<class T>
class Rect {
 public:
  static_assert(std::is_same_v<T, int> || std::is_same_v<T, float>);
  using Vec = typename std::conditional<std::is_same_v<T, int>, glm::ivec2, glm::vec2>::type;

 public:
  Rect() : m_origin(), m_size()
  {

  }

  Rect(const Vec& origin, const Vec& size)
    : m_origin(origin),
      m_size(size)
  {

  };

  Rect(T originX, T originY, T width, T height)
    : m_origin(originX, originY),
      m_size(width, height)
  {

  }

  [[nodiscard]] bool isPointInRect(const Vec& point) const
  {
    return m_origin.x <= point.x && point.x <= (m_origin.x + m_size.x) &&
      m_origin.y <= point.y && point.y <= (m_origin.y + m_size.y);
  }

  [[nodiscard]] T getWidth() const
  {
    return m_size.x;
  }

  [[nodiscard]] T getHeight() const
  {
    return m_size.y;
  }

  [[nodiscard]] const Vec& getSize() const
  {
    return m_size;
  }

  [[nodiscard]] T getOriginX() const
  {
    return m_origin.x;
  }

  [[nodiscard]] T getOriginY() const
  {
    return m_origin.y;
  }

  [[nodiscard]] const Vec& getOrigin() const
  {
    return m_origin;
  }

 private:
  Vec m_origin;
  Vec m_size;
};

using RectI = Rect<int>;
using RectF = Rect<float>;
