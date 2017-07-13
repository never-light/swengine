#include "PlayerSpaceshipController.h"

PlayerSpaceshipController::PlayerSpaceshipController() : SpaceshipController() {

}

PlayerSpaceshipController::~PlayerSpaceshipController() {

}

void PlayerSpaceshipController::initialize(ResourceManager* resMgr) {
	SpaceshipController::initialize(resMgr);

	m_spaceship = new Spaceship(resMgr->loadSprite("resources/sprites/spaceship.png"));
	m_spaceship->initialize();
	m_spaceship->setPosition({ 450, 450 });
}

void PlayerSpaceshipController::update(InputManager* inputMgr) {
	SpaceshipController::update(inputMgr);

	if (inputMgr->isKeyPressed(GLFW_KEY_D)) {
		m_spaceship->move({ 10.0f, 0 });
	}

	if (inputMgr->isKeyPressed(GLFW_KEY_A)) {
		m_spaceship->move({ -10.0f, 0 });
	}
}

void PlayerSpaceshipController::draw(Renderer* renderer) {
	m_spaceship->draw(renderer);
}

void PlayerSpaceshipController::shutdown() {
	m_spaceship->shutdown();
}