#pragma once

#include <Engine/types.h>
#include <Engine/Components/Math/types.h>
#include <Engine/Components/Physics/Colliders/Sphere.h>

class Light {
public:
	enum class Type {
		Point
	};
public:
	Light(Type type);
	virtual ~Light();

	void setType(Type);
	Type getType() const;

	void setColor(const vector3& color);
	vector3 getColor() const;

	void setDiffuseIntensity(float intensity);
	float getDiffuseIntensity() const;

	void setAmbientIntensity(float intensity);
	float getAmbientIntensity() const;

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

	void setBoundingRadius(float radius);
	float getBoundingRadius() const;

private:
	Type m_type;

	vector3 m_position;
	vector3 m_direction;

	vector3 m_color;

	float m_diffuseIntensity;
	float m_ambientIntensity;

	float m_innerAngle;
	float m_outerAngle;

	float m_constant;
	float m_linear;
	float m_quadratic;

	float m_boundingRadius;
};