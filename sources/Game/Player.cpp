#include "Player.h"
#include <Engine\Components\Math\Math.h>
#include <Engine\Components\Debugging\Log.h>
#include <Engine\ServiceLocator.h>

Player::Player(Scene* mainScene)
	: m_mainScene(mainScene),
	m_renderer(ServiceLocator::getRenderer()),
	m_resourceManager(ServiceLocator::getResourceManager()),
	m_inputManager(ServiceLocator::getInputManager()),
	m_viewType(ViewType::Free)
{
	m_thirdPersonCameraDistance = 2.0f;
	m_thirdPersonCameraPitch = 45.0f; 
	m_thirdPersonCameraYaw = 0.0f;

	m_cameraSpeed = 0.15f;
	m_mouseSensitivity = 0.15f;

	// Load player model
	auto playerModel = m_mainScene->createModel("resources/models/player.model", "player");
	playerModel->setMaterial(m_resourceManager->getMaterial("player"));

	// Create player scene node
	m_playerSceneNode = m_mainScene->getRootSceneNode()
		->getChildSceneNode("level")
		->createChildSceneNode("player");
	m_playerSceneNode->attachObject(playerModel);

	m_playerSceneNode->setPosition(-6.83, 2, 2.013);
	m_playerSceneNode->setScale({ 0.3f, 0.3f, 0.3f });
	m_playerSceneNode->lookAt(0, 0, 0);

	// First person camera
	m_firstPersonCamera = new Camera();
	m_firstPersonCamera->setFOVy(45.0f);
	m_firstPersonCamera->setAspectRatio(
		static_cast<float32>(m_renderer->getWindow()->getWidth()) / static_cast<float32>(m_renderer->getWindow()->getWidth())
	);
	m_firstPersonCamera->setNearClipDistance(0.3f);
	m_firstPersonCamera->setFarClipDistance(100.0f);
	m_firstPersonCamera->setPosition(m_playerSceneNode->getPosition());

	// Third person camera
	m_thirdPersonCamera = new Camera();
	m_thirdPersonCamera->setFOVy(45.0f);
	m_thirdPersonCamera->setAspectRatio(
		static_cast<float32>(m_renderer->getWindow()->getWidth()) / static_cast<float32>(m_renderer->getWindow()->getWidth())
	);
	m_thirdPersonCamera->setNearClipDistance(0.3f);
	m_thirdPersonCamera->setFarClipDistance(100.0f);
	m_thirdPersonCamera->setPosition(0, 2, 5);
}

Player::~Player() {
	delete m_firstPersonCamera;
	delete m_thirdPersonCamera;
}

void Player::enableFirstPersonView() {
	m_viewType = ViewType::FirstPerson;
	m_renderer->setCurrentCamera(m_firstPersonCamera);

	m_playerSceneNode->fixYAxis();
}

void Player::enableThirdPersonView() {
	m_viewType = ViewType::ThirdPerson;
	m_renderer->setCurrentCamera(m_thirdPersonCamera);

	vector3 lookPoint = m_playerSceneNode->getPosition() + m_playerSceneNode->getFrontDirection();
	lookPoint.y = m_playerSceneNode->getPosition().y;
	m_playerSceneNode->lookAt(lookPoint);

	m_playerSceneNode->fixYAxis();
}

void Player::disableViews() {
	m_viewType = ViewType::Free;

	m_playerSceneNode->fixYAxis(false);
}

void Player::update() {
	MousePosition pos = m_inputManager->getMousePosition();

	if (m_viewType == ViewType::FirstPerson) {
		real currentPitchAngle = glm::degrees(glm::angle(vector3(0, 1, 0), glm::normalize(m_firstPersonCamera->getFrontDirection())));
		real pitchOffset = m_mouseSensitivity * pos.y * -1.0f;

		if ((pitchOffset > 0 && currentPitchAngle - pitchOffset > 0.001) || (pitchOffset < 0 && currentPitchAngle - pitchOffset < 179.999))
			m_firstPersonCamera->pitch(pitchOffset);

		m_firstPersonCamera->yaw(m_mouseSensitivity * pos.x * -1.0f);

		vector3 offset;

		if (m_inputManager->isKeyPressed(GLFW_KEY_W)) {
			vector3 front = m_playerSceneNode->getFrontDirection();
			front.y = 0;

			offset += glm::normalize(front) * m_cameraSpeed;
		}
		else if (m_inputManager->isKeyPressed(GLFW_KEY_S)) {
			offset += -m_playerSceneNode->getFrontDirection() * m_cameraSpeed;
		}

		if (m_inputManager->isKeyPressed(GLFW_KEY_A)) {
			offset += -m_playerSceneNode->getRightDirection() * m_cameraSpeed;
		}
		else if (m_inputManager->isKeyPressed(GLFW_KEY_D)) {
			offset += m_playerSceneNode->getRightDirection() * m_cameraSpeed;
		}

		offset.y = 0;
		m_playerSceneNode->move(offset);

		m_firstPersonCamera->setPosition(m_playerSceneNode->getPosition());
		m_playerSceneNode->lookAt(m_firstPersonCamera->getPosition() + m_firstPersonCamera->getFrontDirection());
	}
	else if (m_viewType == ViewType::ThirdPerson) {
		if (m_inputManager->getMouseButtonState(GLFW_MOUSE_BUTTON_MIDDLE) == MouseButtonState::Pressed) {
			m_thirdPersonCameraDistance += 0.005 * pos.y * -1.0f;
			m_thirdPersonCameraDistance = glm::clamp(m_thirdPersonCameraDistance, 0.5f, 5.0f);

		}
		else if (m_inputManager->getMouseButtonState(GLFW_MOUSE_BUTTON_LEFT) == MouseButtonState::Pressed) {
			real pitchOffset = m_mouseSensitivity * pos.x * -1.0f;

			m_thirdPersonCameraYaw += pitchOffset;

			m_thirdPersonCameraYaw = glm::clamp(m_thirdPersonCameraYaw, 0.0f, 360.0f);
		}
		else {
			real pitchOffset = m_mouseSensitivity * pos.y * -1.0f;

			m_thirdPersonCameraPitch += pitchOffset;
			m_thirdPersonCameraPitch = glm::clamp(m_thirdPersonCameraPitch, 0.001f, 89.999f);

			m_playerSceneNode->yaw(m_mouseSensitivity * pos.x * -1.0f);
			m_thirdPersonCameraYaw = 0.0f;
		}

		if (m_inputManager->isKeyPressed(GLFW_KEY_W)) {
			m_playerSceneNode->move(m_playerSceneNode->getFrontDirection() * m_cameraSpeed);
		}
		else if (m_inputManager->isKeyPressed(GLFW_KEY_S)) {
			m_playerSceneNode->move(-m_playerSceneNode->getFrontDirection() * m_cameraSpeed);
		}

		if (m_inputManager->isKeyPressed(GLFW_KEY_A)) {
			m_playerSceneNode->move(-m_playerSceneNode->getRightDirection() * m_cameraSpeed);
		}
		else if (m_inputManager->isKeyPressed(GLFW_KEY_D)) {
			m_playerSceneNode->move(m_playerSceneNode->getRightDirection() * m_cameraSpeed);
		}

		vector3 cameraPosition = m_playerSceneNode->getPosition();

		real yawValue = glm::orientedAngle(vector3(1, 0, 0), glm::normalize(m_playerSceneNode->getRightDirection()), vector3(0, 1, 0));
		yawValue += glm::radians(m_thirdPersonCameraYaw);

		real vDist = m_thirdPersonCameraDistance * glm::sin(glm::radians(m_thirdPersonCameraPitch));
		real hDist = m_thirdPersonCameraDistance * glm::cos(glm::radians(m_thirdPersonCameraPitch));

		cameraPosition.y += vDist;
		cameraPosition.z += hDist * glm::cos(yawValue);
		cameraPosition.x += hDist * glm::sin(yawValue);

		m_thirdPersonCamera->setPosition(cameraPosition);
		m_thirdPersonCamera->lookAt(m_playerSceneNode->getPosition());	
	}
}

void Player::render() {
	m_renderer->drawModel((Model*)m_playerSceneNode->getAttachedObjects()[0]);
}