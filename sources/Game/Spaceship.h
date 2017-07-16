#pragma once

#include "Entity.h"

class Spaceship : public Entity {
public:
	Spaceship(Sprite*, const glm::vec2&);
	~Spaceship();

	void draw(Renderer*);

	glm::vec2 getSize() const;
private:
	Sprite* m_spaceshipSprite;
	glm::vec2 m_size;
};