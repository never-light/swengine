#include "Rect.h"

Rect::Rect(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	: m_position(x, y), m_size(width, height)
{
}

Rect::Rect(const uivector2 & position, const uivector2 & size)
	: m_position(position), m_size(size)
{
}

Rect::Rect()
	: m_position(), m_size()
{
}

Rect::~Rect()
{
}

unsigned int Rect::getX() const
{
	return m_position.x;
}

void Rect::setX(unsigned int x)
{
	m_position.x = x;
}

unsigned int Rect::getY() const
{
	return m_position.y;
}

void Rect::setY(unsigned int y)
{
	m_position.y = y;
}

uivector2 Rect::getPosition() const
{
	return m_position;
}

void Rect::setPosition(unsigned int x, unsigned int y)
{
	m_position.x = x;
	m_position.y = y;
}

void Rect::setPosition(const uivector2 & position)
{
	m_position = position;
}

unsigned int Rect::getWidth() const
{
	return m_size.x;
}

void Rect::setWidth(unsigned int width)
{
	m_size.x = width;
}

unsigned int Rect::getHeight() const
{
	return m_size.y;
}

void Rect::setHeight(unsigned int height)
{
	m_size.y = height;
}

uivector2 Rect::getSize() const
{
	return m_size;
}

void Rect::setSize(unsigned int width, unsigned int height)
{
	m_size.x = width;
	m_size.y = height;
}
