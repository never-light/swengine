#pragma once

#include <Engine\Components\Graphics\RenderSystem\Framebuffer.h>
#include "OpenGL3.h"

class OpenGL3Framebuffer : public Framebuffer {
public:
	OpenGL3Framebuffer(int width, int height, const std::vector<Texture*>& textureBuffers);
	~OpenGL3Framebuffer();

	GLuint getFramebufferPointer() const;
	GLuint getDepthBufferPointer() const;

	const std::vector<Texture*>& getTextureBuffers() const override;
private:
	std::vector<Texture*> m_textureBuffers;

	GLuint m_framebufferPointer;
	GLuint m_depthbufferPointer;
};