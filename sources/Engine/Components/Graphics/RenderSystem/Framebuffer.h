#pragma once

#include <unordered_map>
#include "Texture.h"

enum class FramebufferTextureType {
	Depth, Stencil, DepthStencil, Color0, Color1, Color2
};

enum class RenderbufferType {
	Depth, DepthStencil
};

using FramebufferAttachedTexturesList = std::unordered_map<FramebufferTextureType, Texture*>;

class Framebuffer {
public:
	Framebuffer() {}
	virtual ~Framebuffer() {}

	virtual void lock() = 0;
	virtual void unlock() = 0;

	virtual void attachTexture(FramebufferTextureType type, Texture* texture) = 0;
	virtual void createAndAttachRenderBuffer(RenderbufferType type, int width, int height) = 0;

	virtual const FramebufferAttachedTexturesList& getAttachedTextures() const = 0;
	virtual Texture* getAttachedTexture(FramebufferTextureType type) const = 0;
};