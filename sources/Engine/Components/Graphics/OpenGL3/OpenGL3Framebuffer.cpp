#include "OpenGL3Framebuffer.h"
#include "OpenGL3Texture.h"

#include <Engine\Components\Debugging\Log.h>

OpenGL3Framebuffer::OpenGL3Framebuffer(int width, int height, const std::vector<Texture*>& textureBuffers)
	: Framebuffer(width, height, textureBuffers),
	m_textureBuffers(textureBuffers) 
{
	int textureBuffersCount = textureBuffers.size();

	glGenFramebuffers(1, &m_framebufferPointer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferPointer);

	GLenum* buffers = new GLenum[textureBuffersCount];

	for (size_t i = 0; i < textureBuffersCount; i++) {
		buffers[i] = GL_COLOR_ATTACHMENT0 + i;

		GLuint texturePointer = static_cast<OpenGL3Texture*>(textureBuffers[i])->getTexturePointer();
		glBindTexture(GL_TEXTURE_2D, texturePointer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, buffers[i], GL_TEXTURE_2D, texturePointer, 0);
	}

	glDrawBuffers(textureBuffersCount, buffers);

	glGenRenderbuffers(1, &m_depthbufferPointer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthbufferPointer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthbufferPointer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		errlog() << "Creating framebuffer error";
		throw std::exception();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

OpenGL3Framebuffer::~OpenGL3Framebuffer() {
	glDeleteRenderbuffers(1, &m_depthbufferPointer);
	glDeleteRenderbuffers(1, &m_framebufferPointer);
}

GLuint OpenGL3Framebuffer::getFramebufferPointer() const {
	return m_framebufferPointer;
}

GLuint OpenGL3Framebuffer::getDepthBufferPointer() const {
	return m_depthbufferPointer;
}

const std::vector<Texture*>& OpenGL3Framebuffer::getTextureBuffers() const {
	return m_textureBuffers;
}