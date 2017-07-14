#pragma once

#include "State.h"

#include "../PlayerSpaceshipController.h"
#include "../AsteroidsController.h"

class PlayState : public State {
public:
	PlayState();
	~PlayState();

	void load(ResourceManager*);
	void update(InputManager*);
	void draw(Renderer*);
	void unload(ResourceManager*);

private:
	PlayerSpaceshipController* m_playerSpaceshipController;
	AsteroidsController* m_asteroidsController;
};