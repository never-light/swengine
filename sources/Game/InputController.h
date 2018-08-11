#pragma once

#include <Engine\Components\Graphics\RenderSystem\Camera.h>
#include <Engine\Components\InputManager\InputManager.h>

class InputController {
public:
	InputController(InputManager* inputManager);
	~InputController();

	virtual void update() = 0;

protected:
	InputManager * m_inputManager;
};