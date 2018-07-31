#include "Triangle.h"

Triangle::Triangle()
	: m_vertex1(vector3()), m_vertex2(vector3()), m_vertex3(vector3())
{
}

Triangle::Triangle(const vector3 & v1, const vector3 & v2, const vector3 & v3)
	: m_vertex1(v1), m_vertex2(v2), m_vertex3(v3)
{
}

Triangle::~Triangle()
{
}

void Triangle::setVertex1(const vector3 & vertex)
{
	m_vertex1 = vertex;
}

vector3 Triangle::getVertex1() const
{
	return m_vertex1;
}

void Triangle::setVertex2(const vector3 & vertex)
{
	m_vertex2 = vertex;
}

vector3 Triangle::getVertex2() const
{
	return m_vertex2;
}

void Triangle::setVertex3(const vector3 & vertex)
{
	m_vertex3 = vertex;
}

vector3 Triangle::getVertex3() const
{
	return m_vertex3;
}

bool Triangle::isRayIntersecting(const Ray & ray, float& distance)
{
	const float eps = 1e-6;
	vector3 edge1, edge2, h, s, q;

	float a, f, u, v;

	edge1 = m_vertex2 - m_vertex1;
	edge2 = m_vertex3 - m_vertex1;
	h = glm::cross(ray.getDirection(), edge2);

	a = glm::dot(edge1, h);
	if (a > -eps && a < eps)
		return false;

	f = 1 / a;
	s = ray.getOrigin() - m_vertex1;
	u = f * (glm::dot(s, h));

	if (u < 0.0 || u > 1.0)
		return false;

	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.getDirection(), q);
	if (v < 0.0 || u + v > 1.0)
		return false;

	distance = f * glm::dot(edge2, q);

	return distance > eps;
}

bool Triangle::isRayIntersecting(const Ray & ray)
{
	float distance;

	return isRayIntersecting(ray, distance);
}
