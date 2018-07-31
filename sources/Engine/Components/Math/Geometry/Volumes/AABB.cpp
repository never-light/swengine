#include "AABB.h"

#include <utility>

AABB::AABB()
	: m_min(vector3()), m_max(vector3())
{
}

AABB::AABB(const vector3 & min, const vector3 & max)
	: m_min(min), m_max(max)
{
}

AABB::~AABB()
{
}

void AABB::setMin(const vector3 & min)
{
	m_min = min;
}

vector3 AABB::getMin() const
{
	return m_max;
}

void AABB::setMax(const vector3 & max)
{
	m_max = max;
}

vector3 AABB::getMax() const
{
	return m_max;
}

bool AABB::isRayIntersecting(const Ray & ray)
{
	vector3 rayOrigin = ray.getOrigin();
	vector3 rayDir = ray.getDirection();

	vector3 boxMin = m_min;
	vector3 boxMax = m_max;

	float tmin = (boxMin.x - rayOrigin.x) / rayDir.x;
	float tmax = (boxMax.x - rayOrigin.x) / rayDir.x;

	if (tmin > tmax)
		std::swap(tmin, tmax);

	float tymin = (boxMin.y - rayOrigin.y) / rayDir.y;
	float tymax = (boxMax.y - rayOrigin.y) / rayDir.y;

	if (tymin > tymax)
		std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (boxMin.z - rayOrigin.z) / rayDir.z;
	float tzmax = (boxMax.z - rayOrigin.z) / rayDir.z;

	if (tzmin > tzmax)
		std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return true;
}
