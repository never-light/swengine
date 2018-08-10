#include "PlayerController.h"

#include <Engine\assertions.h>

PlayerController::PlayerController(Player * player, Camera * camera, InputManager * inputManager, const std::vector<Animation*>& statesAnimations)
	: m_player(player), 
	m_playerCamera(camera), 
	m_inputManager(inputManager),
	m_statesAnimations(statesAnimations),
	m_playerAnimator(nullptr)
{
	_assert(statesAnimations.size() == PLAYER_STATES_COUNT);

	m_playerAnimator = new Animator(m_player->getSkeleton());
	changeState(PlayerState::Idle);

	inputManager->registerEventListener(this);
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
	m_playerAnimator->update(0.0333f);

	bool needMove = false;

	MousePosition mousePosition = m_inputManager->getMousePosition();

	vector3 position = m_playerCamera->getTransform()->getPosition();
	float oldY = position.y;

	if (m_inputManager->isKeyPressed(GLFW_KEY_W)) {
		position += m_playerCamera->getTransform()->getFrontDirection() * m_movementSpeed;
		needMove = true;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_S)) {
		position -= m_playerCamera->getTransform()->getFrontDirection() * m_movementSpeed;
		needMove = true;
	}

	if (m_inputManager->isKeyPressed(GLFW_KEY_A)) {
		position -= m_playerCamera->getTransform()->getRightDirection() * m_movementSpeed;
		needMove = true;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_D)) {
		position += m_playerCamera->getTransform()->getRightDirection() * m_movementSpeed;
		needMove = true;
	}

	real currentPitchAngle = glm::degrees(glm::angle(vector3(0, 1, 0), glm::normalize(m_playerCamera->getTransform()->getFrontDirection())));
	real pitchOffset = m_mouseSensitivity * mousePosition.y * -1.0f;

	if ((pitchOffset > 0 && currentPitchAngle - pitchOffset > 0.001) || (pitchOffset < 0 && currentPitchAngle - pitchOffset < 179.999))
		m_playerCamera->getTransform()->pitch(pitchOffset);

	position.y = oldY;

	m_playerCamera->getTransform()->yaw(m_mouseSensitivity * mousePosition.x * -1.0f);
	m_playerCamera->getTransform()->setPosition(position);

	m_player->getTransform()->setOrientation(m_playerCamera->getTransform()->getOrientation());
	m_player->getTransform()->yaw(179.9f);

	m_player->getTransform()->setPosition(m_playerCamera->getTransform()->getPosition() - vector3(0, 0.4f, 0.0f));

	if (needMove && m_currentPlayerState != PlayerState::Running)
		changeState(PlayerState::Idle);
	
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
		changeState(PlayerState::Taking);
	}
}

void PlayerController::changeState(PlayerState state)
{
	m_currentPlayerState = state;

	Animation* newStateAnimation = m_statesAnimations[(size_t)state];
	m_playerAnimator->setCurrentAnimation(newStateAnimation);
}
