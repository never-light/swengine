#include "PlayerController.h"

#include <Engine\assertions.h>
#include <Game\config.h>

PlayerController::PlayerController(Player * player, Camera * camera, InputManager * inputManager, const std::vector<Animation*>& statesAnimations)
	: InputController(inputManager), 
	m_player(player), 
	m_playerCamera(camera), 
	m_statesAnimations(statesAnimations),
	m_playerAnimator(nullptr)
{
	_assert(statesAnimations.size() == PLAYER_STATES_COUNT);

	inputManager->registerEventListener(this);

	m_player->getTransform()->fixYAxis();

	m_playerAnimator = new Animator(m_player->getSkeleton());
	changeState(PlayerState::Idle);
	m_playerAnimator->play();
}

PlayerController::~PlayerController()
{
	if (m_playerAnimator != nullptr) {
		delete m_playerAnimator;
		m_playerAnimator = nullptr;
	}
}

void PlayerController::update()
{
	if (m_currentPlayerState == PlayerState::Taking && m_playerAnimator->isStopped()) {
		changeState(PlayerState::Idle);
		m_playerAnimator->play();
	}

	m_playerAnimator->increaseAnimationTime(1.0f / GAME_STATE_UPDATES_PER_SECOND);

	m_player->applyPose(m_playerAnimator->getAnimatedPose());

	bool needMove = false;

	MousePosition mousePosition = m_inputManager->getMousePosition();

	vector3 position = m_player->getTransform()->getPosition();
	float oldY = position.y;

	if (m_inputManager->isKeyPressed(GLFW_KEY_W)) {
		position += m_player->getTransform()->getFrontDirection() * m_movementSpeed;
		needMove = true;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_S)) {
		position -= m_player->getTransform()->getFrontDirection() * m_movementSpeed;
		needMove = true;
	}

	if (m_inputManager->isKeyPressed(GLFW_KEY_A)) {
		position -= m_player->getTransform()->getRightDirection() * m_movementSpeed;
		needMove = true;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_D)) {
		position += m_player->getTransform()->getRightDirection() * m_movementSpeed;
		needMove = true;
	}

	real currentPitchAngle = glm::degrees(glm::angle(vector3(0, 1, 0), glm::normalize(m_player->getTransform()->getFrontDirection())));
	real pitchOffset = m_mouseSensitivity * mousePosition.y * -1.0f;

	if ((pitchOffset > 0 && currentPitchAngle - pitchOffset > 0.001) || (pitchOffset < 0 && currentPitchAngle - pitchOffset < 179.999))
		m_player->getTransform()->pitch(pitchOffset);

	position.y = oldY;

	m_player->getTransform()->yaw(m_mouseSensitivity * mousePosition.x * -1.0f);
	m_player->getTransform()->setPosition(position);

	if (needMove && m_currentPlayerState != PlayerState::Running)
		changeState(PlayerState::Running);
	
	if (!needMove && m_currentPlayerState == PlayerState::Running)
		changeState(PlayerState::Idle);
}

Camera * PlayerController::getAttachedCamera() const
{
	return m_playerCamera;
}

void PlayerController::setMovementSpeed(float speed)
{
	m_movementSpeed = speed;
}

float PlayerController::getMovementSpeed() const
{
	return m_movementSpeed;
}

void PlayerController::setMouseSensitivity(float sensitivity)
{
	m_mouseSensitivity = sensitivity;
}

float PlayerController::getMouseSensitivity() const
{
	return m_mouseSensitivity;
}

PlayerController::PlayerState PlayerController::getCurrentPlayerState() const
{
	return m_currentPlayerState;
}

void PlayerController::onKeyPress(Key key, KeyEvent::Modifier mod)
{
	if (key == GLFW_KEY_F) {
		if (!m_inputManager->isKeyPressed(GLFW_KEY_W) && 
			!m_inputManager->isKeyPressed(GLFW_KEY_A) &&
			!m_inputManager->isKeyPressed(GLFW_KEY_S) &&
			!m_inputManager->isKeyPressed(GLFW_KEY_D)
			) 
		{
			changeState(PlayerState::Taking);
		}
	}
}

void PlayerController::changeState(PlayerState state)
{
	m_currentPlayerState = state;

	Animation* newStateAnimation = m_statesAnimations[(size_t)state];
	m_playerAnimator->setCurrentAnimation(newStateAnimation);
}
