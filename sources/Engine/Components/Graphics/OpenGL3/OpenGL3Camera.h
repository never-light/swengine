#pragma once

#include <Engine\Components\Graphics\RenderSystem\Camera.h>

class OpenGL3Camera : public Camera {
public:
	OpenGL3Camera();
	~OpenGL3Camera();

	void setPosition(const vector3&) override;
	void setPosition(float32, float32, float32) override;
	void move(const vector3&) override;
	void move(float32, float32, float32) override;
	const vector3& getPosition() const override;
	
	void setOrientation(const quaternion&) override;
	const quaternion& getOrientation() const override;
	void rotate(const quaternion&, CoordinateSystemType relativeTo = CoordinateSystemType::Local) override;
	void rotate(const vector3&, float32, CoordinateSystemType relativeTo = CoordinateSystemType::Local) override;
	void yaw(float32) override;
	void pitch(float32) override;
	void roll(float32) override;

	vector3 getForwardDirection() const override;
	vector3 getRightDirection() const override;
	vector3 getUpDirection() const override;

	void setAspectRatio(float32) override;
	float32 getAspectRatio() const override;

	void setNearClipDistance(float32) override;
	float32 getNearClipDistance() const override;

	void setFarClipDistance(float32) override;
	float32 getFarClipDistance() const override;

	void setFOVy(float32) override;
	float32 getFOVy() const override;

	virtual matrix4 getViewMatrix() const override;
	matrix4 getProjectionMatrix() const override;
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
