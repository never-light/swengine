#pragma once

#include <vector>
#include <map>

#include <Engine\Components\Graphics\RenderSystem\Material.h>
#include "OpenGL3Texture.h"
#include "OpenGL3GpuProgram.h"

class OpenGL3Material : public Material {
public:
	OpenGL3Material();
	~OpenGL3Material();

	void setGpuProgram(GpuProgram* program) override;
	GpuProgram* getGpuProgram() const override;

	void setLightmap(LightmapType, Texture*) override;
	Texture* getLightmap(LightmapType) const override;
	bool hasLightmap(LightmapType) const override;

	void setSpecularColor(const Color&) override;
	Color getSpecularColor() const override;

	void setDuffuseColor(const Color&) override;
	Color getDuffuseColor() const override;

	void setAmbientColor(const Color&) override;
	Color getAmbientColor() const override;

	void setShininess(float32) override;
	float32 getShininess() const override;

	void setParameter(const std::string& name, MaterialParameter value) override;
	MaterialParameter getParameter(const std::string& name) const override;
	bool hasParameter(const std::string& name) const override;
private:
	std::map<std::string, MaterialParameter> m_parameters;

	std::map<LightmapType, OpenGL3Texture*> m_lightmaps;
	OpenGL3GpuProgram* m_shader;

	Color m_diffuseColor;
	Color m_specularColor;
	Color m_ambientColor;

	float32 m_shininess;
};