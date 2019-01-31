#pragma once

#include <Engine/types.h>
#include <Engine/Components/Math/Transform.h>

class Camera {
public:
	Camera();
	virtual ~Camera();

	Transform* getTransform() const;

	void setAspectRatio(float32);
	float32 getAspectRatio() const;

	void setNearClipDistance(float32);
	float32 getNearClipDistance() const;

	void setFarClipDistance(float32);
	float32 getFarClipDistance() const;

	void setFOVy(float32);
	float32 getFOVy() const;

	matrix4 getProjectionMatrix() const;
	matrix4 getViewMatrix() const;
private:
	// View space transform
	Transform* m_transform;

	// Projection parameters
	float32 m_FOVy;
	float32 m_aspectRatio;
	float32 m_nearClipDistance;
	float32 m_farClipDistance;
};
