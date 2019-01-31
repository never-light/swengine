#pragma once

class GameWorld;

class GameSystem {
public:
	GameSystem();
	virtual ~GameSystem();

	virtual void update(GameWorld* gameWorld, float delta);
	virtual void render(GameWorld* gameWorld);

	virtual void configure(GameWorld* gameWorld);
	virtual void unconfigure(GameWorld* gameWorld);
};