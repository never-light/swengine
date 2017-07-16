#pragma once

#include <string>
#include <list>
#include <vector>

#include <Engine\Components\Math\Math.h>
#include <Engine\Components\ResourceManager\ResourceManager.h>
#include <Engine\Components\InputManager\InputManager.h>
#include <Engine\Components\Graphics\Graphics.h>

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