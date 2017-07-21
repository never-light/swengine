#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>

class OpenGL3Camera {
public:
	OpenGL3Camera();
	OpenGL3Camera(const vector3&);
	OpenGL3Camera(const vector3&, const quaternion&);

	~OpenGL3Camera();

	vector3 getPosition() const;
	quaternion getOrientation() const;

	void setPosition(const vector3&);
	void setPosition(float32, float32, float32);

	void setOrientation(const quaternion&);

	void move(const vector3&);
	void move(float32, float32, float32);

	void rotate(const quaternion&);
	void rotate(float32, const vector3&);
	void rotate(float32, float32, float32, float32);

	void yaw(float32);
	void pitch(float32);
	void roll(float32);

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
	vector3 m_position;
	quaternion m_orientation;

	float32 m_FOVy;
	float32 m_aspectRatio;
	float32 m_nearClipDistance;
	float32 m_farClipDistance;
};