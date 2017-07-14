#include "Spaceship.h"

Spaceship::Spaceship(Sprite* spaceshipSprite, const glm::vec2& size) 
	: m_spaceshipSprite(spaceshipSprite), m_size(size) {

}

Spaceship::~Spaceship() {

}

void Spaceship::draw(Renderer* renderer) {
	renderer->drawSprite(m_spaceshipSprite, m_position, m_size, m_rotation);
}

glm::vec2 Spaceship::getSize() const {
	return m_size;
}