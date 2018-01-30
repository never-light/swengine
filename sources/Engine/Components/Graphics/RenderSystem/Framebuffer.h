#pragma once

#include <vector>
#include "Texture.h"

class Framebuffer {
public:
	Framebuffer(int width, int height, const std::vector<Texture*>& textureBuffers) {}
	virtual ~Framebuffer() {}

	virtual const std::vector<Texture*>& getTextureBuffers() const = 0;
};