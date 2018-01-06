#pragma once

#include <Engine\Components\Graphics\RenderSystem\Light.h>

class OpenGL3Light : public Light {
public:
	OpenGL3Light();
	virtual ~OpenGL3Light();

	void setType(LightType) override;
	LightType getType() const override;

	void setAmbientColor(const Color&) override;
	Color getAmbientColor() const override;

	void setSpecularColor(const Color&) override;
	Color getSpecularColor() const override;

	void setDuffuseColor(const Color&) override;
	Color getDuffuseColor() const override;

	void setDirection(const vector3&) override;
	vector3 getDirection() const override;

	void setPosition(const vector3&) override;
	vector3 getPosition() const override;

	void setAttenuation(float32, float32, float32) override;

	float32 getAttenuationConstant() const override;
	float32 getAttenuationLinear() const override;
	float32 getAttenuationQuadratic() const override;

	void setInnerAngle(float32) override;
	float32 getInnerAngle() const override;

	void setOuterAngle(float32) override;
	float32 getOuterAngle() const override;
protected:
	LightType m_type;

	vector3 m_position;
	vector3 m_direction;

	Color m_ambientColor;
	Color m_specularColor;
	Color m_diffuseColor;

	float32 m_innerAngle;
	float32 m_outerAngle;

	float32 m_constant;
	float32 m_linear;
	float32 m_quadratic;
};