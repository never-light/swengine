#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\SceneManager\SceneObject.h>
#include <Engine\Components\SceneManager\SceneNode.h>

class Camera : public SceneObject {
public:
	Camera() : SceneObject(SceneObjectType::Camera) {}

	virtual void setPosition(const vector3&) = 0;
	virtual void setPosition(float32, float32, float32) = 0;
	virtual void move(const vector3&) = 0;
	virtual void move(float32, float32, float32) = 0;
	virtual const vector3& getPosition() const = 0;

	virtual void setOrientation(const quaternion&) = 0;
	virtual const quaternion& getOrientation() const = 0;
	virtual void rotate(const quaternion&, CoordinateSystemType relativeTo = CoordinateSystemType::Local) = 0;
	virtual void rotate(const vector3&, float32, CoordinateSystemType relativeTo = CoordinateSystemType::Local) = 0;
	virtual void yaw(float32) = 0;
	virtual void pitch(float32) = 0;
	virtual void roll(float32) = 0;

	virtual vector3 getForwardDirection() const = 0;
	virtual vector3 getRightDirection() const = 0;
	virtual vector3 getUpDirection() const = 0;

	virtual void setAspectRatio(float32) = 0;
	virtual float32 getAspectRatio() const = 0;

	virtual void setNearClipDistance(float32) = 0;
	virtual float32 getNearClipDistance() const = 0;

	virtual void setFarClipDistance(float32) = 0;
	virtual float32 getFarClipDistance() const = 0;

	virtual void setFOVy(float32) = 0;
	virtual float32 getFOVy() const = 0;

	virtual matrix4 getViewMatrix() const = 0;
	virtual matrix4 getProjectionMatrix() const = 0;
};
