#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>

class OpenGL3Camera {
public:
	OpenGL3Camera();
	//OpenGL3Camera(const vector3&, const vector3&, const vector3&);
	virtual ~OpenGL3Camera();

	vector3 getPosition() const;

	void setPosition(const vector3&);
	void setPosition(float32, float32, float32);

	void move(const vector3&);
	void move(float32, float32, float32);

	void lookAt(const vector3&);

	void setMaxPitchValue(float32);
	float32 getMaxPitchValue() const;

	void yaw(float32);
	void pitch(float32);

	matrix4 getOrientationMatrix() const;

	vector3 getForwardDirection() const;
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

	matrix4 getViewMatrix() const;
	matrix4 getProjectionMatrix() const;
private:
	void normalizeAngles();

private:
	vector3 m_position;
	float32 m_yaw;
	float32 m_pitch;
	float32 m_maxPitchValue;

	float32 m_FOVy;
	float32 m_aspectRatio;
	float32 m_nearClipDistance;
	float32 m_farClipDistance;
};