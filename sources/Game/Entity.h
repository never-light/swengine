#pragma once

#include "../Engine/Math.h"
#include "../Engine/Renderer/OpenGL3Renderer.h"

class Entity {
public:
	Entity();
	~Entity();

	virtual void draw(Renderer*);

	void setPosition(const glm::vec2&);
	glm::vec2 getPosition() const;
	void move(const glm::vec2&);

	float getRotation() const;
	void setRotation(float);
	void rotate(float);
protected:
	glm::vec2 m_position;
	float m_rotation;
};