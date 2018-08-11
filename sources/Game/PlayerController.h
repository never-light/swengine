#pragma once

#include "Player.h"
#include "InputController.h"

#include <Game\Graphics\Animation\Animator.h>

class PlayerController : public InputController, public InputEventsListener {
public:
	enum class PlayerState {
		Idle = 0, Running = 1, Taking = 2
	};

	static const size_t PLAYER_STATES_COUNT = 3;
public:
	PlayerController(Player* player, Camera* camera, InputManager* inputManager, const std::vector<Animation*>& statesAnimations);
	~PlayerController();

	void update();

	Camera* getAttachedCamera() const;
	
	void setMovementSpeed(float speed);
	float getMovementSpeed() const;

	void setMouseSensitivity(float sensitivity);
	float getMouseSensitivity() const;

	PlayerState getCurrentPlayerState() const;

	virtual void onKeyPress(Key key, KeyEvent::Modifier mod);
private:
	void changeState(PlayerState state);

private:
	PlayerState m_currentPlayerState;

	float m_movementSpeed = 0.10f;
	float m_mouseSensitivity = 0.15f;

	std::vector<Animation*> m_statesAnimations;
	Animator* m_playerAnimator;

private:
	Player* m_player;
	Camera* m_playerCamera;
};