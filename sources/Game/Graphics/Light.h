#pragma once

#include <Engine\types.h>
#include <Engine\Components\Math\types.h>
#include <Engine\Components\Physics\Colliders\Sphere.h>

class Light {
public:
	enum class Type {
		Directional = 1, Point = 2, Spotlight = 3
	};
public:
	Light(Type type);
	virtual ~Light();

	void setType(Type);
	Type getType() const;

	void setSpecularColor(const vector3&);
	vector3 getSpecularColor() const;

	void setDuffuseColor(const vector3&);
	vector3 getDuffuseColor() const;

	void setDirection(const vector3&);
	vector3 getDirection() const;

	void setPosition(const vector3&);
	vector3 getPosition() const;

	void setAttenuation(float constant, float linear, float quadratic);

	float getAttenuationConstant() const;
	float getAttenuationLinear() const;
	float getAttenuationQuadratic() const;

	void setInnerAngle(float);
	float getInnerAngle() const;

	void setOuterAngle(float);
	float getOuterAngle() const;

	void setBoundingVolume(const Sphere& volume);
	Sphere getBoundingVolume();
private:
	Type m_type;

	vector3 m_position;
	vector3 m_direction;

	vector3 m_ambientColor;
	vector3 m_specularColor;
	vector3 m_diffuseColor;

	float m_innerAngle;
	float m_outerAngle;

	float m_constant;
	float m_linear;
	float m_quadratic;

	Sphere m_boundingVolume;
};