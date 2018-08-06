#pragma once

#include <Engine\Components\Math\types.h>

struct Intersection {
public:
	Intersection();
	Intersection(const vector3& direction, float depth);
	~Intersection();

	vector3 getDirection() const;
	void setDirection(const vector3& direction);

	float getDepth() const;
	void setDepth(float depth);

private:
	vector3 m_direction;
	float m_depth;
};