#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>

class Camera {
public:
	Camera();
	virtual ~Camera();

	void setPosition(const vector3&);
	void setPosition(float32, float32, float32);
	void move(const vector3&);
	void move(float32, float32, float32);
	const vector3& getPosition() const;

	void setYawValue(real value);
	void setPitchValue(real value);
	void setRollValue(real value);

	void yaw(real angle);
	void pitch(real angle);
	void roll(real angle);

	real getYawValue() const;
	real getPitchValue() const;
	real getRollValue() const;

	void lookAt(const vector3& target);
	void lookAt(real x, real y, real z);

	vector3 getFrontDirection() const;
	vector3 getRightDirection() const;
	vector3 getUpDirection() const;

	void setAspectRatio(float32);
	float32 getAspectRatio() const;

	void setNearClipDistance(float32);
	float32 getNearClipDistance() const;

	void setFarClipDistance(float32);
	float32 getFarClipDistance() const;

	void setFOVy(float32);
	float32 getFOVy() const;

	virtual matrix4 getViewMatrix() const ;
	virtual matrix4 getProjectionMatrix() const;

private:
	// View space parameters
	vector3 m_position;
	quaternion m_orientation;

	// Projection parameters
	float32 m_FOVy;
	float32 m_aspectRatio;
	float32 m_nearClipDistance;
	float32 m_farClipDistance;
};
