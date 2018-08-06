#pragma once

#include <Engine\Components\Math\types.h>
#include <Engine\Components\Physics\Ray.h>

class Sphere {
public:
	Sphere();
	Sphere(const vector3& center, float radius);
	~Sphere();

	vector3 getCenter() const;
	void setCenter(const vector3& center);

	float getRadius() const;
	void setRadius(float radius);

	bool isRayIntersecting(const Ray& ray);
private:
	vector3 m_center;
	float m_radius;
};