#include "CameraController.h"

CameraController::CameraController()
	: m_inputMgr(nullptr), m_renderer(nullptr)
{

}

CameraController::~CameraController() {

}

void CameraController::initialize(InputManager* inputMgr, Renderer* renderer) {
	m_inputMgr = inputMgr;
	m_renderer = renderer;

	m_cameraSpeed = 0.15f;
	m_mouseSensitivity = 0.15f;

	m_camera = new OpenGL3Camera();
	m_camera->setFOVy(45.0f);
	m_camera->setAspectRatio(
		static_cast<float32>(m_renderer->getWindow()->getWidth()) / static_cast<float32>(m_renderer->getWindow()->getWidth())
	);
	m_camera->setNearClipDistance(0.3f);
	m_camera->setFarClipDistance(100.0f);
	m_camera->setPosition(0, 0, 5);
	m_renderer->setCurrentCamera(m_camera);
}

void CameraController::shutdown() {
	delete m_camera;
}

void CameraController::update() {
	MousePosition pos = m_inputMgr->getMousePosition();

	m_camera->yaw(m_mouseSensitivity * pos.x * -1.0f);
	m_camera->pitch(m_mouseSensitivity * pos.y * -1.0f);

	if (m_inputMgr->isKeyPressed(GLFW_KEY_W)) {
		m_camera->move(m_camera->getForwardDirection() * m_cameraSpeed);
	}
	else if (m_inputMgr->isKeyPressed(GLFW_KEY_S)) {
		m_camera->move(-m_camera->getForwardDirection() * m_cameraSpeed);
	}

	if (m_inputMgr->isKeyPressed(GLFW_KEY_A)) {
		m_camera->move(-m_camera->getRightDirection() * m_cameraSpeed);
	}
	else if (m_inputMgr->isKeyPressed(GLFW_KEY_D)) {
		m_camera->move(m_camera->getRightDirection() * m_cameraSpeed);
	}
}