#pragma once

#include <unordered_map>
#include <Engine\Components\Graphics\RenderSystem\Framebuffer.h>
#include "OpenGL3.h"

class OpenGL3Framebuffer : public Framebuffer {
public:
	OpenGL3Framebuffer();
	~OpenGL3Framebuffer();

	GLuint getFramebufferPointer() const;

	void lock() override;
	void unlock() override;

	void attachTexture(FramebufferTextureType type, Texture* texture) override;
	void createAndAttachRenderBuffer(RenderbufferType type, int width, int height) override;

	const FramebufferAttachedTexturesList& getAttachedTextures() const override;
	Texture* getAttachedTexture(FramebufferTextureType type) const override;
private:
	void freeData();
private:
	std::unordered_map<FramebufferTextureType, Texture*> m_attachedTextures;
	std::unordered_map<RenderbufferType, GLuint> m_attachedRenderbuffers;

	GLuint m_framebufferPointer;
};