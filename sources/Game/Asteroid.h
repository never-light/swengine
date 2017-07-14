#pragma once

#include "Entity.h"
#include "../Engine/Renderer/OpenGL3Sprite.h"
#include "../Engine/Math.h"

class Asteroid : public Entity {
public:
	Asteroid(Sprite*, const glm::vec2&);
	~Asteroid();

	void draw(Renderer*);

	glm::vec2 getSize() const;

	glm::vec2 getVelocity() const;
	void setVelocity(const glm::vec2&);
private:
	Sprite* m_asteroidSpite;
	glm::vec2 m_size;
	glm::vec2 m_velocity;
};