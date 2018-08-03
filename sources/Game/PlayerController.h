#pragma once

#include "Player.h"

#include <Engine\Components\InputManager\InputManager.h>

class PlayerController : public InputEventsListener {
public:
	PlayerController(Player* player, Camera* camera, InputManager* inputManager);
	~PlayerController();

	void update();

	Camera* getAttachedCamera() const;
	
	void setMovementSpeed(float speed);
	float getMovementSpeed() const;

	void setMouseSensitivity(float sensitivity);
	float getMouseSensitivity() const;

private:
	float m_movementSpeed = 0.10f;
	float m_mouseSensitivity = 0.15f;

private:
	Player * m_player;
	Camera* m_playerCamera;

private:
	InputManager * m_inputManager;
};