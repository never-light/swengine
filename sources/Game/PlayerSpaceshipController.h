#pragma once

#include "SpaceshipController.h"

class PlayerSpaceshipController : public SpaceshipController {
public:
	PlayerSpaceshipController();
	~PlayerSpaceshipController();

	virtual void initialize(ResourceManager*);
	virtual void update(InputManager*);
	virtual void draw(Renderer*);
	virtual void shutdown();
};