#pragma once

#include <Engine\Components\SceneManager\Scene.h>
#include <Engine\Components\InputManager\InputManager.h>
#include "SolidGameObject.h"
#include "PlayerController.h"

class StartScene : public Scene, public InputEventsListener {
public:
	StartScene(GraphicsContext* graphicsContext, ResourceManager* resourceManager, InputManager* inputManager);
	virtual ~StartScene();

	virtual void update() override;
	virtual void render() override;

protected:
	void loadResources();
	void initializeSceneObjects();

protected:
	SolidGameObject* m_level;

	Player* m_player;
	PlayerController* m_playerController;
	Camera* m_playerCamera;
protected:
	GpuProgram* m_lightingGpuProgram;

protected:
	InputManager* m_inputManager;
};