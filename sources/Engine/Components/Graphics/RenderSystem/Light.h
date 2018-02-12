#pragma once

#include <Engine\types.h>
#include "Color.h"

enum class LightType {
	Directional = 1, Point = 2, Spotlight = 3
};

class Light {
public:
	Light(LightType type);
	virtual ~Light();

	virtual void setType(LightType);
	virtual LightType getType() const;

	virtual void setAmbientColor(const Color&);
	virtual Color getAmbientColor() const;

	virtual void setSpecularColor(const Color&);
	virtual Color getSpecularColor() const;

	virtual void setDuffuseColor(const Color&);
	virtual Color getDuffuseColor() const;

	virtual void setDirection(const vector3&);
	virtual vector3 getDirection() const;

	virtual void setPosition(const vector3&);
	virtual vector3 getPosition() const;

	virtual void setAttenuation(real constant, real linear, real quadratic);

	virtual real getAttenuationConstant() const;
	virtual real getAttenuationLinear() const;
	virtual real getAttenuationQuadratic() const;

	virtual void setInnerAngle(real);
	virtual real getInnerAngle() const;

	virtual void setOuterAngle(real);
	virtual real getOuterAngle() const;

	virtual const matrix4& getSpaceMatrix();
private:
	LightType m_type;

	vector3 m_position;
	vector3 m_direction;

	Color m_ambientColor;
	Color m_specularColor;
	Color m_diffuseColor;

	real m_innerAngle;
	real m_outerAngle;

	real m_constant;
	real m_linear;
	real m_quadratic;

	bool m_needUpdateSpaceMatrix;
	matrix4 m_spaceMatrix;
};