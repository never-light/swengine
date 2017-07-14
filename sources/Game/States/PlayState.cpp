#include "PlayState.h"

PlayState::PlayState() : m_asteroidsController(nullptr), m_playerSpaceshipController(nullptr) {

}

PlayState::~PlayState() {

}

void PlayState::load(ResourceManager* resMgr) {
	m_asteroidsController = new AsteroidsController();
	m_asteroidsController->initialize(resMgr);

	m_playerSpaceshipController = new PlayerSpaceshipController();
	m_playerSpaceshipController->initialize(resMgr);
}

void PlayState::update(InputManager* inputMgr) {
	m_asteroidsController->update(inputMgr);
	m_playerSpaceshipController->update(inputMgr);
}

void PlayState::draw(Renderer* renderer) {
	m_asteroidsController->draw(renderer);
	m_playerSpaceshipController->draw(renderer);
}

void PlayState::unload(ResourceManager* resMgr) {
	m_asteroidsController->shutdown();
	m_playerSpaceshipController->shutdown();
}