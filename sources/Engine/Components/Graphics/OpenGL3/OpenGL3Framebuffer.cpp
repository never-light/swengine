#include "OpenGL3Framebuffer.h"
#include "OpenGL3Texture.h"

#include <Engine\Components\Debugging\Log.h>

OpenGL3Framebuffer::OpenGL3Framebuffer()
	: Framebuffer()
{

}

OpenGL3Framebuffer::~OpenGL3Framebuffer() {
	freeData();
}

GLuint OpenGL3Framebuffer::getFramebufferPointer() const {
	return m_framebufferPointer;
}

void OpenGL3Framebuffer::lock() {
	freeData();

	glGenFramebuffers(1, &m_framebufferPointer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferPointer);
}
void OpenGL3Framebuffer::unlock() {
	std::vector<GLuint> colorAttachments;

	auto color0 = m_attachedTextures.find(FramebufferTextureType::Color0);
	if (color0 != m_attachedTextures.end())
		colorAttachments.push_back(GL_COLOR_ATTACHMENT0);

	auto color1 = m_attachedTextures.find(FramebufferTextureType::Color1);
	if (color1 != m_attachedTextures.end())
		colorAttachments.push_back(GL_COLOR_ATTACHMENT1);

	auto color2 = m_attachedTextures.find(FramebufferTextureType::Color2);
	if (color2 != m_attachedTextures.end())
		colorAttachments.push_back(GL_COLOR_ATTACHMENT2);

	if (colorAttachments.size()) {
		glDrawBuffers(colorAttachments.size(), colorAttachments.data());
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		errlog() << "Creating framebuffer error";
		throw std::exception();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGL3Framebuffer::attachTexture(FramebufferTextureType type, Texture* texture) {
	OpenGL3Texture* glTexture = static_cast<OpenGL3Texture*>(texture);

	if (glTexture->getType() != Texture::Type::_2D)
		throw std::exception();

	GLenum attachmentType;

	if (type == FramebufferTextureType::Color0) {
		attachmentType = GL_COLOR_ATTACHMENT0;
	}
	else if (type == FramebufferTextureType::Color1) {
		attachmentType = GL_COLOR_ATTACHMENT1;
	}
	else if (type == FramebufferTextureType::Color2) {
		attachmentType = GL_COLOR_ATTACHMENT2;
	}
	else if (type == FramebufferTextureType::Depth) {
		attachmentType = GL_DEPTH_ATTACHMENT;
	}
	else if (type == FramebufferTextureType::Stencil) {
		attachmentType = GL_STENCIL_ATTACHMENT;
	}
	else if (type == FramebufferTextureType::DepthStencil) {
		attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
	}

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, glTexture->getBindTarget(), glTexture->getTexturePointer(), 0);

	m_attachedTextures[type] = texture;
}

void OpenGL3Framebuffer::createAndAttachRenderBuffer(RenderbufferType type, int width, int height) {
	auto it = m_attachedRenderbuffers.find(type);
	if (it != m_attachedRenderbuffers.end()) {
		glDeleteRenderbuffers(1, &it->second);
	}

	GLenum internalFormat;
	GLenum attachmentType;

	if (type == RenderbufferType::Depth) {
		internalFormat = GL_DEPTH_COMPONENT;
		attachmentType = GL_DEPTH_ATTACHMENT;
	}
	else if (type == RenderbufferType::DepthStencil) {
		internalFormat = GL_DEPTH24_STENCIL8;
		attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
	}

	GLuint renderBufferPointer;

	glGenRenderbuffers(1, &renderBufferPointer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferPointer);
	glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, renderBufferPointer);

	m_attachedRenderbuffers[type] = renderBufferPointer;
}

void OpenGL3Framebuffer::freeData() {
	for (auto renderBuffer : m_attachedRenderbuffers)
		glDeleteRenderbuffers(1, &renderBuffer.second);

	m_attachedRenderbuffers.clear();

	glDeleteFramebuffers(1, &m_framebufferPointer);
}

const FramebufferAttachedTexturesList& OpenGL3Framebuffer::getAttachedTextures() const {
	return m_attachedTextures;
}

Texture* OpenGL3Framebuffer::getAttachedTexture(FramebufferTextureType type) const {
	return m_attachedTextures.at(type);
}