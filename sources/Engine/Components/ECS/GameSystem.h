#pragma once

class GameWorld;

class GameSystem {
public:
	GameSystem();
	virtual ~GameSystem();

	virtual void update(GameWorld* gameWorld, float delta) = 0;

	virtual void configure(GameWorld* gameWorld);
	virtual void unconfigure(GameWorld* gameWorld);
};