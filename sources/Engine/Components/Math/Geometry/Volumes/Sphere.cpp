#include "Sphere.h"

Sphere::Sphere()
	: m_center(), m_radius(0.0f)
{
}

Sphere::Sphere(const vector3 & center, float radius)
	: m_center(center), m_radius(radius)
{

}

Sphere::~Sphere()
{
}

vector3 Sphere::getCenter() const
{
	return m_center;
}

void Sphere::setCenter(const vector3 & center)
{
	m_center = center;
}

float Sphere::getRadius() const
{
	return m_radius;
}

void Sphere::setRadius(float radius)
{
	m_radius = radius;
}

bool Sphere::isRayIntersecting(const Ray& ray)
{
	float t0, t1;

	vector3 l = m_center - ray.getOrigin();
	float tca = glm::dot(l, ray.getDirection());

	float d2 = glm::dot(l, l) - tca * tca;
	if (d2 > m_radius) return false;
	float thc = sqrt(m_radius - d2);
	t0 = tca - thc;
	t1 = tca + thc;

	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0) {
		t0 = t1;
		if (t0 < 0) return false;
	}

	return true;

}
