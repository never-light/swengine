#include "Spaceship.h"

Spaceship::Spaceship(Sprite* spaceshipSprite) : m_spaceshipSprite(spaceshipSprite) {

}

Spaceship::~Spaceship() {

}

void Spaceship::initialize() {
	m_spaceshipSprite->setSize({ 100, 100 });
}

void Spaceship::draw(Renderer* renderer) {
	renderer->drawSprite(m_spaceshipSprite);
}

void Spaceship::shutdown() {

}

void Spaceship::setPosition(const glm::vec2& position) {
	m_spaceshipSprite->setPosition(position);
}

glm::vec2 Spaceship::getPosition() const {
	return m_spaceshipSprite->getPosition();
}

void Spaceship::move(const glm::vec2& movement) {
	m_spaceshipSprite->setPosition(m_spaceshipSprite->getPosition() + movement);
}