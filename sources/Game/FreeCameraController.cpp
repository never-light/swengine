#include "FreeCameraController.h"

FreeCameraController::FreeCameraController(Camera * camera, InputManager * inputManager)
	: InputController(inputManager), 
	m_camera(camera)
{
}

FreeCameraController::~FreeCameraController()
{
}

void FreeCameraController::update()
{
	MousePosition mousePosition = m_inputManager->getMousePosition();

	vector3 position = m_camera->getTransform()->getPosition();
	float oldY = position.y;

	if (m_inputManager->isKeyPressed(GLFW_KEY_W)) {
		position += m_camera->getTransform()->getFrontDirection() * m_movementSpeed;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_S)) {
		position -= m_camera->getTransform()->getFrontDirection() * m_movementSpeed;
	}

	if (m_inputManager->isKeyPressed(GLFW_KEY_A)) {
		position -= m_camera->getTransform()->getRightDirection() * m_movementSpeed;
	}
	else if (m_inputManager->isKeyPressed(GLFW_KEY_D)) {
		position += m_camera->getTransform()->getRightDirection() * m_movementSpeed;
	}

	real currentPitchAngle = glm::degrees(glm::angle(vector3(0, 1, 0), glm::normalize(m_camera->getTransform()->getFrontDirection())));
	real pitchOffset = m_mouseSensitivity * mousePosition.y * -1.0f;

	if ((pitchOffset > 0 && currentPitchAngle - pitchOffset > 0.001) || (pitchOffset < 0 && currentPitchAngle - pitchOffset < 179.999))
		m_camera->getTransform()->pitch(pitchOffset);

	m_camera->getTransform()->yaw(m_mouseSensitivity * mousePosition.x * -1.0f);
	m_camera->getTransform()->setPosition(position);
}

Camera * FreeCameraController::getAttachedCamera() const
{
	return m_camera;
}

void FreeCameraController::setMovementSpeed(float speed)
{
	m_movementSpeed = speed;
}

float FreeCameraController::getMovementSpeed() const
{
	return m_movementSpeed;
}

void FreeCameraController::setMouseSensitivity(float sensitivity)
{
	m_mouseSensitivity = sensitivity;
}

float FreeCameraController::getMouseSensitivity() const
{
	return m_mouseSensitivity;
}