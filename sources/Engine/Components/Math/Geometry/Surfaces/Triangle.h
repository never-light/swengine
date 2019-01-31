#pragma once

#include <Engine/Components/Math/types.h>
#include <Engine/Components/Physics/Ray.h>

class Triangle {
public:
	Triangle();
	Triangle(const vector3& v1, const vector3& v2, const vector3& v3);
	~Triangle();

	void setVertex1(const vector3& vertex);
	vector3 getVertex1() const;

	void setVertex2(const vector3& vertex);
	vector3 getVertex2() const;

	void setVertex3(const vector3& vertex);
	vector3 getVertex3() const;

	bool isRayIntersecting(const Ray& ray, float& distance);
	bool isRayIntersecting(const Ray& ray);
private:
	vector3 m_vertex1;
	vector3 m_vertex2;
	vector3 m_vertex3;
};