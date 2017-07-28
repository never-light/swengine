#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "OpenGL3Light.h"

class OpenGL3PointLight : public OpenGL3Light {
public:
	OpenGL3PointLight();
	~OpenGL3PointLight();

	void setPosition(const vector3&);
	vector3 getPosition() const;

	void setAttenuation(float32, float32, float32);

	float32 getAttenuationConstant() const;
	float32 getAttenuationLinear() const;
	float32 getAttenuationQuadratic() const;

private:
	using OpenGL3Light::setType;

private:
	vector3 m_position;
	float32 m_constant;
	float32 m_linear;
	float32 m_quadratic;
};