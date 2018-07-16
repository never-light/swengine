#pragma once

#include "types.h"

class Transform
{
public:
	Transform();

	void move(float x, float y, float z);
	void move(vector3 movement);

	void setPosition(float x, float y, float z);
	void setPosition(const vector3& position);
	vector3 getPosition() const;

	void scale(float x, float y, float z);
	void scale(const vector3& scale);

	void setScale(float x, float y, float z);
	void setScale(const vector3& scale);
	vector3 getScale() const;

	void rotate(float x, float y, float z, float angle);
	void rotate(const vector3& axis, float angle);

	void setOrientation(const quaternion& orientation);
	quaternion getOrientation() const;

	vector3 getFrontDirection() const;
	vector3 getRightDirection() const;
	vector3 getUpDirection() const;

	void fixYAxis(bool fixed = true);
	bool isYAxisFixed() const;

	void yaw(float angle);
	void pitch(float angle);
	void roll(float angle);

	float getYawValue() const;
	float getPitchValue() const;
	float getRollValue() const;

	void lookAt(float x, float y, float z);
	void lookAt(const vector3& target);

	matrix4 getTransformationMatrix() const;
protected:
	vector3 m_position;
	vector3 m_scale;
	quaternion m_orientation;

	bool m_fixedYAxis;
};