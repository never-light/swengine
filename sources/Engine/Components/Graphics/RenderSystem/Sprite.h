#pragma once

#include "GpuProgram.h"
#include "Texture.h"

class Sprite {
public:
	virtual void setTexture(Texture*) = 0;
	virtual const Texture* getTexture() const = 0;

	virtual void setShader(GpuProgram*) = 0;
	virtual GpuProgram* getShader() const = 0;
};