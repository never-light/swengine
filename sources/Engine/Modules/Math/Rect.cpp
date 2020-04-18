#include "precompiled.h"

#pragma hdrstop

#include "Rect.h"

Rect::Rect()
  : m_origin(),
    m_size()
{

}

Rect::Rect(const Rect& rect)
  : m_origin(rect.m_origin),
    m_size(rect.m_size)
{
}

Rect::Rect(const glm::ivec2& origin, const glm::ivec2& size)
  : m_origin(origin),
    m_size(size)
{
  SW_ASSERT(size.x >= 0 && size.y >= 0);
}

Rect::Rect(int originX, int originY, int width, int height)
  : m_origin(originX, originY),
    m_size(width, height)
{
  SW_ASSERT(width >= 0 && height >= 0);
}

int Rect::getWidth() const
{
  return m_size.x;
}

int Rect::getHeight() const
{
  return m_size.y;
}

const glm::ivec2& Rect::getSize() const
{
  return m_size;
}

int Rect::getOriginX() const
{
  return m_origin.x;
}

int Rect::getOriginY() const
{
  return m_origin.y;
}

bool Rect::isPointInRect(const glm::ivec2& point) const
{
  return m_origin.x <= point.x && point.x <= (m_origin.x + m_size.x) &&
    m_origin.y <= point.y && point.y <= (m_origin.y + m_size.y);;
}

const glm::ivec2& Rect::getOrigin() const
{
  return m_origin;
}
