#include "PlayerSpaceshipController.h"
#include "config.h"

PlayerSpaceshipController::PlayerSpaceshipController() : SpaceshipController() {

}

PlayerSpaceshipController::~PlayerSpaceshipController() {

}

void PlayerSpaceshipController::initialize(ResourceManager* resMgr) {
	SpaceshipController::initialize(resMgr);

	m_spaceship = new Spaceship(resMgr->loadSprite("resources/sprites/playerShip1_blue.png"), { 100, 75 });
	m_spaceship->setPosition({
		WIDTH_VALUE/2 - m_spaceship->getSize().x/2,
		HEIGHT_VALUE - m_spaceship->getSize().y - 15
	});
}

void PlayerSpaceshipController::update(InputManager* inputMgr) {
	SpaceshipController::update(inputMgr);

	if (inputMgr->isKeyPressed(GLFW_KEY_D)) {
		if (m_spaceship->getPosition().x < WIDTH_VALUE - m_spaceship->getSize().x) {
			m_spaceship->move({ 10.0f, 0 });
		}
	}

	if (inputMgr->isKeyPressed(GLFW_KEY_A)) {
		if (m_spaceship->getPosition().x > 0) {
			m_spaceship->move({ -10.0f, 0 });
		}
	}
}

void PlayerSpaceshipController::draw(Renderer* renderer) {
	m_spaceship->draw(renderer);
}

void PlayerSpaceshipController::shutdown() {

}