#pragma once

#include <Engine\types.h>

#include "GpuProgram.h"
#include "Texture.h"
#include "Color.h"

enum class LightmapType {
	Diffuse, Specular
};

class Material {
public:
	virtual void setGpuProgram(GpuProgram* program) = 0;
	virtual GpuProgram* getGpuProgram() const = 0;

	virtual void setLightmap(LightmapType, Texture*) = 0;
	virtual Texture* getLightmap(LightmapType) const = 0;
	virtual bool hasLightmap(LightmapType) const = 0;

	virtual void setSpecularColor(const Color&) = 0;
	virtual Color getSpecularColor() const = 0;

	virtual void setDuffuseColor(const Color&) = 0;
	virtual Color getDuffuseColor() const = 0;

	virtual void setAmbientColor(const Color&) = 0;
	virtual Color getAmbientColor() const = 0;

	virtual void setShininess(float32) = 0;
	virtual float32 getShininess() const = 0;
};