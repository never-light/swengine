#pragma once

#include "../Engine/ResourceManager/ResourceManager.h"
#include "../Engine/InputManager/InputManager.h"
#include "../Engine/Renderer/OpenGL3Renderer.h"

#include "Spaceship.h"

class SpaceshipController {
public:
	SpaceshipController();
	virtual ~SpaceshipController();

	virtual void initialize(ResourceManager*);
	virtual void update(InputManager*);
	virtual void draw(Renderer*);
	virtual void shutdown();
protected:
	Spaceship* m_spaceship;
};