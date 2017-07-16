#pragma once

#include <Engine\Components\Math\Math.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>
#include <Engine\Components\InputManager\InputManager.h>
#include <Engine\Components\Graphics\Graphics.h>

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