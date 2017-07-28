#pragma once

#include <vector>
#include <map>

#include <Engine\types.h>
#include <Engine\Components\Graphics\Color.h>
#include "OpenGL3Texture.h"
#include "OpenGL3Lightmap.h"
#include "OpenGL3Shader.h"

class OpenGL3Material {
public:
	OpenGL3Material();
	~OpenGL3Material();

	void setShader(OpenGL3Shader*);
	OpenGL3Shader* getShader() const;

	void setLightmap(LightmapType, OpenGL3Texture*);
	OpenGL3Texture* getLightmap(LightmapType) const;
	bool isLightmapExists(LightmapType) const;

	void setSpecularColor(const Color&);
	Color getSpecularColor() const;

	void setDuffuseColor(const Color&);
	Color getDuffuseColor() const;

	void setAmbientColor(const Color&);
	Color getAmbientColor() const;

	void setShininess(float32);
	float32 getShininess() const;
private:
	std::map<LightmapType, OpenGL3Texture*> m_lightmaps;
	OpenGL3Shader* m_shader;

	Color m_diffuseColor;
	Color m_specularColor;
	Color m_ambientColor;

	float32 m_shininess;
};