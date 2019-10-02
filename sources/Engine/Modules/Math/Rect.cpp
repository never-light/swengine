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

const glm::ivec2& Rect::getOrigin() const
{
    return m_origin;
}
