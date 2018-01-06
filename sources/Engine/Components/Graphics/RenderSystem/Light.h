#pragma once

#include <Engine\types.h>
#include "Color.h"

enum class LightType {
	Directional = 1, Point = 2, Spotlight = 3
};

class Light {
public:
	virtual void setType(LightType) = 0;
	virtual LightType getType() const = 0;

	virtual void setAmbientColor(const Color&) = 0;
	virtual Color getAmbientColor() const = 0;

	virtual void setSpecularColor(const Color&) = 0;
	virtual Color getSpecularColor() const = 0;

	virtual void setDuffuseColor(const Color&) = 0;
	virtual Color getDuffuseColor() const = 0;

	virtual void setDirection(const vector3&) = 0;
	virtual vector3 getDirection() const = 0;

	virtual void setPosition(const vector3&) = 0;
	virtual vector3 getPosition() const = 0;

	virtual void setAttenuation(float32, float32, float32) = 0;

	virtual float32 getAttenuationConstant() const = 0;
	virtual float32 getAttenuationLinear() const = 0;
	virtual float32 getAttenuationQuadratic() const = 0;

	virtual void setInnerAngle(float32) = 0;
	virtual float32 getInnerAngle() const = 0;

	virtual void setOuterAngle(float32) = 0;
	virtual float32 getOuterAngle() const = 0;
};