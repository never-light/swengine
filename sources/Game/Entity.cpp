#include "Entity.h"

Entity::Entity() {

}

Entity::~Entity() {

}

void Entity::draw(Renderer* renderer) {

}

void Entity::setPosition(const glm::vec2& position) {
	m_position = position;
}

glm::vec2 Entity::getPosition() const {
	return m_position;
}

void Entity::move(const glm::vec2& movement) {
	m_position += movement;
}

float Entity::getRotation() const {
	return m_rotation;
}

void Entity::setRotation(float rotation) {
	m_rotation = rotation;
}

void Entity::rotate(float rotation) {
	m_rotation += rotation;

	if (m_rotation > 360) {
		m_rotation -= 360;
	}
	else if (m_rotation < 0) {
		m_rotation += 360;
	}
}