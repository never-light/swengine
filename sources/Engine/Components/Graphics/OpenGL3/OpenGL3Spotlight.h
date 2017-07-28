#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\Math.h>
#include "OpenGL3Light.h"

class OpenGL3Spotlight : public OpenGL3Light {
public:
	OpenGL3Spotlight();
	~OpenGL3Spotlight();

	void setPosition(const vector3&);
	vector3 getPosition() const;

	void setDirection(const vector3&);
	vector3 getDirection() const;

	void setInnerAngle(float32);
	float32 getInnerAngle() const;

	void setOuterAngle(float32);
	float32 getOuterAngle() const;

	void setAttenuation(float32, float32, float32);

	float32 getAttenuationConstant() const;
	float32 getAttenuationLinear() const;
	float32 getAttenuationQuadratic() const;

private:
	using OpenGL3Light::setType;

private:
	vector3 m_position;
	vector3 m_direction;

	float32 m_innerAngle;
	float32 m_outerAngle;

	float32 m_constant;
	float32 m_linear;
	float32 m_quadratic;
};