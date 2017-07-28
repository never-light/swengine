#pragma once

#include <Engine\Components\Math\Math.h>
#include <Engine\Components\Graphics\Color.h>

enum class LightType {
	Directional = 1, Point = 2, Spotlight = 3
};

class OpenGL3Light {
public:
	OpenGL3Light();
	virtual ~OpenGL3Light();

	void setType(LightType);
	LightType getType() const;

	void setAmbientColor(const Color&);
	Color getAmbientColor() const;

	void setSpecularColor(const Color&);
	Color getSpecularColor() const;

	void setDuffuseColor(const Color&);
	Color getDuffuseColor() const;
protected:
	LightType m_type;

	vector3 position;
	Color m_ambientColor;
	Color m_specularColor;
	Color m_diffuseColor;
};