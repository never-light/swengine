#pragma once

#include "State.h"

class MainMenuState : public State {
public:
	MainMenuState();
	~MainMenuState();
	
	void load(ResourceManager*);
	void update(InputManager*);
	void draw(Renderer*);
	void unload(ResourceManager*);
};