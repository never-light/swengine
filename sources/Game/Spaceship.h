#pragma once

#include "../Engine/Renderer/OpenGL3Sprite.h"
#include "../Engine/Renderer/OpenGL3Renderer.h"
#include "Math.h"

class Spaceship {
public:
	Spaceship(Sprite*);
	~Spaceship();

	void initialize();
	void draw(Renderer*);
	void shutdown();

	void setPosition(const glm::vec2&);
	glm::vec2 getPosition() const;
	void move(const glm::vec2&);
private:
	Sprite* m_spaceshipSprite;
};