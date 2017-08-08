#pragma once

#include <Engine\Components\InputManager\InputManager.h>
#include <Engine\Components\Graphics\Camera.h>
#include <Engine\Components\Graphics\Renderer.h>

class CameraController {
public:
	CameraController();
	~CameraController();

	void initialize(InputManager* inputMgr, Renderer* renderer);
	void shutdown();

	void update();
private:
	Camera* m_camera;

	real m_cameraSpeed;
	real m_mouseSensitivity;
private:
	InputManager* m_inputMgr;
	Renderer* m_renderer;
};