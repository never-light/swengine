#pragma once

#include <Engine\types.h>
#include <Engine\Components\Graphics\Color.h>

class OpenGL3Material {
public:
	OpenGL3Material();
	~OpenGL3Material();

	void setAmbientColor(const Color&);
	Color getAmbientColor() const;

	void setSpecularColor(const Color&);
	Color getSpecularColor() const;

	void setDuffuseColor(const Color&);
	Color getDuffuseColor() const;

	void setShininess(float32);
	float32 getShininess() const;

private:
	Color m_ambientColor;
	Color m_specularColor;
	Color m_diffuseColor;
	float32 m_shininess;
};