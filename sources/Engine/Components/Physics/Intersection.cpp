#include "Intersection.h"

Intersection::Intersection()
	: m_direction(0.0f, 0.0f, 0.0f), m_depth(0.0f)
{
}

Intersection::Intersection(const vector3 & direction, float depth)
	: m_direction(direction), m_depth(depth)
{

}

Intersection::~Intersection()
{
}

vector3 Intersection::getDirection() const
{
	return m_direction;
}

void Intersection::setDirection(const vector3 & direction)
{
	m_direction = direction;
}

float Intersection::getDepth() const
{
	return m_depth;
}

void Intersection::setDepth(float depth)
{
	m_depth = depth;
}
