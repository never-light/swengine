#pragma once

#include "BaseApplication.h"
#include "Player.h"

class Application : public BaseApplication, public InputEventsListener {
public:
	Application(const std::string&, unsigned int, unsigned int);
	~Application();

	void update();
	void render();
private:
	void onKeyPress(int16 key, KeyEvent::Modifier mod) override;

private:
	void toggleViewType(ViewType type);

private:
	Camera* m_freeCamera;
	real m_cameraSpeed;
	real m_mouseSensitivity;
	ViewType m_viewType;

	SceneNode* m_levelNode;
	Scene* m_mainScene;

	Player* m_player;
};