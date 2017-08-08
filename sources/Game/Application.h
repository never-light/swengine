#pragma once

#include "BaseApplication.h"
#include "CameraController.h"

class Application : public BaseApplication, public InputEventsListener {
public:
	Application();
	~Application();

	void initialize(const std::string&, unsigned int, unsigned int);
	void shutdown();

	void update();
	void render();
private:
	void onKeyPress(int16 key, KeyEvent::Modifier mod) override;

private:
	CameraController* cameraController;
	Scene* m_mainScene;
	Material* m_bronzeMaterial;
};