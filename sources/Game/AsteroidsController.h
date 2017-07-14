#pragma once

#include <string>
#include <list>
#include <vector>

#include "../Engine/ResourceManager/ResourceManager.h"
#include "../Engine/InputManager/InputManager.h"
#include "../Engine/Renderer/OpenGL3Renderer.h"
#include "../Engine/Math.h"

#include "Asteroid.h"

class AsteroidsController {
public:
	AsteroidsController();
	~AsteroidsController();

	virtual void initialize(ResourceManager*);
	virtual void update(InputManager*);
	virtual void draw(Renderer*);
	virtual void shutdown();

private:
	void createAsteroidInRandomPlace();
	std::list<Asteroid*> m_asteroidsList;
	std::vector<std::string> m_asteroidsSprites;

	ResourceManager* m_resMgr;

	int m_updatesCount;
};