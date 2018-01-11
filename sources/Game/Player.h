#pragma once

#include <Engine\Components\InputManager\InputManager.h>
#include <Engine\Components\Graphics\RenderSystem\Camera.h>
#include <Engine\Components\Graphics\OpenGL3\OpenGL3Renderer.h>
#include <Engine\Components\SceneManager\Scene.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>

#include "ViewType.h"

class Player {
public:
	Player(Scene* mainScene);
	~Player();

	void update();
	void render();

	void enableFirstPersonView();
	void enableThirdPersonView();
	void disableViews();

private:
	Camera* m_firstPersonCamera;
	Camera* m_thirdPersonCamera;
	ViewType m_viewType;

	real m_cameraSpeed;
	real m_mouseSensitivity;

	real m_thirdPersonCameraDistance;
	real m_thirdPersonCameraPitch;
	real m_thirdPersonCameraYaw;
private:
	Scene* m_mainScene;
	SceneNode* m_playerSceneNode;

private:
	Renderer* m_renderer;
	InputManager* m_inputManager;
	ResourceManager* m_resourceManager;
};