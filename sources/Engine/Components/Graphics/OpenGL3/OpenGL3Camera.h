#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\SceneManager\SceneObject.h>
#include <Engine\Components\SceneManager\SceneNode.h>

class OpenGL3Camera : public SceneObject {
public:
	OpenGL3Camera();
	~OpenGL3Camera();

	void setPosition(const vector3&);
	void setPosition(float32, float32, float32);
	void move(const vector3&);
	void move(float32, float32, float32);
	const vector3& getPosition() const;
	
	void setOrientation(const quaternion&);
	const quaternion& getOrientation() const;
	void rotate(const quaternion&, CoordinateSystemType relativeTo = CoordinateSystemType::Local);
	void rotate(const vector3&, float32, CoordinateSystemType relativeTo = CoordinateSystemType::Local);
	void yaw(float32);
	void pitch(float32);
	void roll(float32);

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

	virtual matrix4 getViewMatrix() const;
	matrix4 getProjectionMatrix() const;
private:
	void normalizeAngles();

private:
	vector3 m_position;
	quaternion m_orientation;

	float32 m_FOVy;
	float32 m_aspectRatio;
	float32 m_nearClipDistance;
	float32 m_farClipDistance;
};
