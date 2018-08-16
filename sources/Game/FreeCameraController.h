#pragma once

#include "InputController.h"

class FreeCameraController : public InputController {
public:
	FreeCameraController(Camera* camera, InputManager* inputManager);
	~FreeCameraController();

	void update();

	Camera* getAttachedCamera() const;

	void setMovementSpeed(float speed);
	float getMovementSpeed() const;

	void setMouseSensitivity(float sensitivity);
	float getMouseSensitivity() const;

private:
	float m_movementSpeed = 0.15f;
	float m_mouseSensitivity = 0.2f;

private:
	Camera* m_camera;
};