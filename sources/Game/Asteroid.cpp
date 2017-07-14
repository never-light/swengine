#include "Asteroid.h"

Asteroid::Asteroid(Sprite* asteroidSprite, const glm::vec2& size) 
	: m_asteroidSpite(asteroidSprite), m_velocity(glm::vec2(0, 0)),
	m_size(size) {
}

Asteroid::~Asteroid() {

}

void Asteroid::draw(Renderer* renderer) {
	renderer->drawSprite(m_asteroidSpite, m_position, m_size, m_rotation);
}

glm::vec2 Asteroid::getSize() const {
	return m_size;
}

glm::vec2 Asteroid::getVelocity() const {
	return m_velocity;
}

void Asteroid::setVelocity(const glm::vec2& velocity) {
	m_velocity = velocity;
}