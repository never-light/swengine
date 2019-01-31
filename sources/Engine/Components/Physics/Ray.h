#pragma once

#include <Engine/Components/Math/types.h>

class Ray
{
public:
    Ray();
    Ray(const vector3& origin, const vector3& direction);

	vector3 getOrigin() const;
    void setOrigin(const vector3& origin);

	vector3 getDirection() const;
    void setDirection(const vector3&direction);

private:
	vector3 m_origin;
	vector3 m_direction;
};