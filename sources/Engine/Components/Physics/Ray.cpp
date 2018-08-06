#include "Ray.h"

Ray::Ray()
{

}

Ray::Ray(const vector3& origin, const vector3& direction)
    : m_origin(origin), m_direction(direction)
{

}

vector3 Ray::getOrigin() const
{
    return m_origin;
}

void Ray::setOrigin(const vector3& origin)
{
    m_origin = origin;
}

vector3 Ray::getDirection() const
{
    return m_direction;
}

void Ray::setDirection(const vector3& direction)
{
    m_direction = direction;
}
