#include "precompiled.h"

#pragma hdrstop

#include "RectF.h"

RectF::RectF()
  : m_origin(),
    m_size()
{

}

RectF::RectF(const RectF& rect)
  : m_origin(rect.m_origin),
    m_size(rect.m_size)
{

}

RectF::RectF(const glm::vec2& origin, const glm::vec2& size)
  : m_origin(origin),
    m_size(size)
{

}

float RectF::getWidth() const
{
  return m_size.x;
}

float RectF::getHeight() const
{
  return m_size.y;
}

const glm::vec2& RectF::getSize() const
{
  return m_size;
}

float RectF::getOriginX() const
{
  return m_origin.x;
}

float RectF::getOriginY() const
{
  return m_origin.y;
}

const glm::vec2& RectF::getOrigin() const
{
  return m_origin;
}
