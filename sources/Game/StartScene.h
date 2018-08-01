#pragma once

#include <Engine\Components\SceneManager\Scene.h>
#include <Engine\Components\InputManager\InputManager.h>
#include "SolidGameObject.h"

class StartScene : public Scene, public InputEventsListener {
public:
	StartScene(GraphicsContext* graphicsContext, ResourceManager* resourceManager, InputManager* inputManager);
	virtual ~StartScene();

	virtual void update() override;
	virtual void render() override;

protected:
	SolidGameObject* m_stalker;
	SolidGameObject* m_house;

protected:
	InputManager* m_inputManager;
};