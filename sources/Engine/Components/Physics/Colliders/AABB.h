#pragma once

#include <Engine/Components/Math/types.h>
#include <Engine/Components/Physics/Ray.h>

class AABB {
public:
	AABB();
	AABB(const vector3& min, const vector3& max);
	~AABB();

	void setMin(const vector3& min);
	vector3 getMin() const;

	void setMax(const vector3& max);
	vector3 getMax() const;

	bool isRayIntersecting(const Ray& ray);

protected:
	vector3 m_min;
	vector3 m_max;
};