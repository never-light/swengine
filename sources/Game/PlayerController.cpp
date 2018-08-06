#include "PlayerController.h"

PlayerController::PlayerController(Player * player, Camera * camera, InputManager * inputManager)
	: m_player(player), m_playerCamera(camera), m_inputManager(inputManager)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::update()
{
	MousePosition mousePosition = m_inputManager->getMousePosition();

	vector3 position = m_playerCamera->getTransform()->getPosition();
	float oldY = position.y;

	if (m_inputManager->isKeyPressed(GLFW_KEY_W))
		position += m_playerCamera->getTransform()->getFrontDirection() * m_movementSpeed;
	else if (m_inputManager->isKeyPressed(GLFW_KEY_S))
		position -= m_playerCamera->getTransform()->getFrontDirection() * m_movementSpeed;

	if (m_inputManager->isKeyPressed(GLFW_KEY_A))
		position -= m_playerCamera->getTransform()->getRightDirection() * m_movementSpeed;
	else if (m_inputManager->isKeyPressed(GLFW_KEY_D))
		position += m_playerCamera->getTransform()->getRightDirection() * m_movementSpeed;

	if (m_inputManager->isKeyPressed(GLFW_KEY_Q))
		m_playerCamera->getTransform()->yaw(1.0f);
	if (m_inputManager->isKeyPressed(GLFW_KEY_E))
		m_playerCamera->getTransform()->yaw(-1.0f);

	real currentPitchAngle = glm::degrees(glm::angle(vector3(0, 1, 0), glm::normalize(m_playerCamera->getTransform()->getFrontDirection())));
	real pitchOffset = m_mouseSensitivity * mousePosition.y * -1.0f;

	if ((pitchOffset > 0 && currentPitchAngle - pitchOffset > 0.001) || (pitchOffset < 0 && currentPitchAngle - pitchOffset < 179.999))
		m_playerCamera->getTransform()->pitch(pitchOffset);

	position.y = oldY;

	m_playerCamera->getTransform()->yaw(m_mouseSensitivity * mousePosition.x * -1.0f);
	m_playerCamera->getTransform()->setPosition(position);

	m_player->getTransform()->setOrientation(m_playerCamera->getTransform()->getOrientation());
	m_player->getTransform()->setPosition(m_playerCamera->getTransform()->getPosition());
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
