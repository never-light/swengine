#include "AsteroidsController.h"
#include "config.h"

AsteroidsController::AsteroidsController() : m_updatesCount(0) {

}

AsteroidsController::~AsteroidsController() {

}

void AsteroidsController::initialize(ResourceManager* resMgr) {
	m_resMgr = resMgr;

	m_asteroidsSprites = std::vector<std::string>{
		{"resources/sprites/Meteors/meteorBrown_big1.png"},
		{ "resources/sprites/Meteors/meteorBrown_big2.png" },
		{ "resources/sprites/Meteors/meteorBrown_big3.png" },
		{ "resources/sprites/Meteors/meteorBrown_big4.png" },
		{ "resources/sprites/Meteors/meteorGrey_big1.png" },
		{ "resources/sprites/Meteors/meteorGrey_big2.png" },
		{ "resources/sprites/Meteors/meteorGrey_big3.png" },
		{ "resources/sprites/Meteors/meteorGrey_big4.png" },
	};
}

void AsteroidsController::update(InputManager* inputMgr) {
	if (m_updatesCount >= GAME_SPEED * 2) {
		createAsteroidInRandomPlace();
		m_updatesCount = 0;
	}

	for (auto asteroid : m_asteroidsList) {
		asteroid->move(asteroid->getVelocity());
	}

	m_updatesCount++;
}

void AsteroidsController::draw(Renderer* renderer) {
	for (auto asteroid : m_asteroidsList) {
		asteroid->draw(renderer);
	}
}

void AsteroidsController::shutdown() {
	for (auto asteroid : m_asteroidsList) {
		delete asteroid;
	}
}

void AsteroidsController::createAsteroidInRandomPlace() {
	std::string spriteName = m_asteroidsSprites.at(Random::getInteger(0, m_asteroidsSprites.size()-1));
	int width = Random::getInteger(50, 100);

	Asteroid* asteroid = new Asteroid(
		m_resMgr->loadSprite(spriteName),
		{static_cast<float>(width), width*1.2}
	);
	asteroid->setPosition({ Random::getInteger(0, 1000), -asteroid->getSize().y - 5 });
	asteroid->setVelocity({Random::getFloat(-0.3, 0.3), Random::getFloat(1.0, 1.5)});

	asteroid->setRotation(static_cast<float>(Random::getInteger(0, 360)));

	m_asteroidsList.push_back(asteroid);
}