#pragma once

#include "Entity.h"
#include "../Engine/Renderer/OpenGL3Sprite.h"
#include "../Engine/Math.h"

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