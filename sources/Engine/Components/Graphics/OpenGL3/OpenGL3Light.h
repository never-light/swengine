#pragma once

#include <Engine\Components\Math\Math.h>
#include <Engine\Components\Graphics\Color.h>

enum LightType {
	LIGHT_POINT
};

class OpenGL3Light {
public:
	OpenGL3Light();
	~OpenGL3Light();

	void setType(LightType);
	LightType getType() const;

	void setAmbientColor(const Color&);
	Color getAmbientColor() const;

	void setSpecularColor(const Color&);
	Color getSpecularColor() const;

	void setDuffuseColor(const Color&);
	Color getDuffuseColor() const;
private:
	LightType m_type;

	vector3 position;
	Color m_ambientColor;
	Color m_specularColor;
	Color m_diffuseColor;
};