#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "OpenGL3Light.h"

class OpenGL3DirectionalLight : public OpenGL3Light {
public:
	OpenGL3DirectionalLight();
	~OpenGL3DirectionalLight();

	void setDirection(const vector3&);
	vector3 getDirection() const;

private:
	using OpenGL3Light::setType;

private:
	vector3 m_direction;
};