#include "OpenGL3Framebuffer.h"
#include "OpenGL3Texture.h"

#include <Engine\Components\Debugging\Log.h>

const std::unordered_map<Framebuffer::Attachment, GLenum> OpenGL3Framebuffer::m_attachmentsTypesMap{
	{ Framebuffer::Attachment::Color0, GL_COLOR_ATTACHMENT0 },
	{ Framebuffer::Attachment::Color1, GL_COLOR_ATTACHMENT1 },
	{ Framebuffer::Attachment::Color2, GL_COLOR_ATTACHMENT2 },
	{ Framebuffer::Attachment::Deptch, GL_DEPTH_ATTACHMENT },
	{ Framebuffer::Attachment::Stencil, GL_STENCIL_ATTACHMENT },
	{ Framebuffer::Attachment::DepthStencil, GL_DEPTH_STENCIL_ATTACHMENT },
};

const std::unordered_map<Framebuffer::RenderBufferInternalFormat, GLenum> OpenGL3Framebuffer::m_renderBufferInternalFormatsMap{
	{ Framebuffer::RenderBufferInternalFormat::Depth, GL_DEPTH_COMPONENT },
	{ Framebuffer::RenderBufferInternalFormat::Depth24Stencil8, GL_DEPTH24_STENCIL8 }
};

const std::unordered_map<Framebuffer::CopyAttachmentMask, GLbitfield> OpenGL3Framebuffer::m_copyAttachmentMasksMap{
	{ Framebuffer::CopyAttachmentMask::ColorBuffer, GL_COLOR_BUFFER_BIT },
	{ Framebuffer::CopyAttachmentMask::DepthBuffer, GL_DEPTH_BUFFER_BIT },
	{ Framebuffer::CopyAttachmentMask::StencilBuffer, GL_STENCIL_BUFFER_BIT },
	{ Framebuffer::CopyAttachmentMask::DepthStencilBuffer, (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT) },
};

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

	auto color0 = m_attachedTextures.find(Framebuffer::Attachment::Color0);
	if (color0 != m_attachedTextures.end())
		colorAttachments.push_back(GL_COLOR_ATTACHMENT0);

	auto color1 = m_attachedTextures.find(Framebuffer::Attachment::Color1);
	if (color1 != m_attachedTextures.end())
		colorAttachments.push_back(GL_COLOR_ATTACHMENT1);

	auto color2 = m_attachedTextures.find(Framebuffer::Attachment::Color2);
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

void OpenGL3Framebuffer::freeData() {
	for (auto renderBuffer : m_attachedRenderbuffers)
		glDeleteRenderbuffers(1, &renderBuffer.second);

	m_attachedRenderbuffers.clear();

	m_attachedTextures.clear();

	glDeleteFramebuffers(1, &m_framebufferPointer);
}

void OpenGL3Framebuffer::attachTexture(Framebuffer::Attachment attachment, Texture* texture) {
	OpenGL3Texture* glTexture = static_cast<OpenGL3Texture*>(texture);

	if (glTexture->getType() != Texture::Type::_2D &&
		glTexture->getType() != Texture::Type::_2DMultisample)
		throw std::exception();

	glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		m_attachmentsTypesMap.at(attachment), 
		glTexture->getBindTarget(), 
		glTexture->getTexturePointer(),
		0
	);

	m_attachedTextures[attachment] = texture;
}

GLuint OpenGL3Framebuffer::createRenderBuffer(
	int width, int height,
	RenderBufferInternalFormat internalFormat,
	bool multisample, int samples
) {
	GLuint renderBufferPointer;

	glGenRenderbuffers(1, &renderBufferPointer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferPointer);

	if (!multisample) {
		glRenderbufferStorage(
			GL_RENDERBUFFER,
			m_renderBufferInternalFormatsMap.at(internalFormat),
			width, height
		);
	}
	else {
		glRenderbufferStorageMultisample(
			GL_RENDERBUFFER,
			samples,
			m_renderBufferInternalFormatsMap.at(internalFormat),
			width, height
		);
	}

	return renderBufferPointer;
}

void OpenGL3Framebuffer::attachRenderBuffer(Attachment attachment, GLuint renderBufferPointer) {
	glFramebufferRenderbuffer(
		GL_FRAMEBUFFER,
		m_attachmentsTypesMap.at(attachment),
		GL_RENDERBUFFER,
		renderBufferPointer
	);
}

void OpenGL3Framebuffer::createAndAttachRenderBuffer(
	Attachment attachment,
	int width,
	int height,
	RenderBufferInternalFormat internalFormat
) {
	if (m_attachedRenderbuffers.find(attachment) != m_attachedRenderbuffers.end())
		throw std::exception();

	GLuint renderBufferPointer = createRenderBuffer(width, height, internalFormat);
	attachRenderBuffer(attachment, renderBufferPointer);

	m_attachedRenderbuffers[attachment] = renderBufferPointer;
}

void OpenGL3Framebuffer::createAndAttachMultisampleRenderBuffer(
	Attachment attachment,
	int samples,
	int width,
	int height,
	RenderBufferInternalFormat internalFormat
) {
	if (m_attachedRenderbuffers.find(attachment) != m_attachedRenderbuffers.end())
		throw std::exception();

	GLuint renderBufferPointer = createRenderBuffer(width, height, internalFormat, true, samples);
	attachRenderBuffer(attachment, renderBufferPointer);

	m_attachedRenderbuffers[attachment] = renderBufferPointer;
}

void OpenGL3Framebuffer::copyAttachmentFragmentTo(
	Attachment attachment,
	Framebuffer* destination,
	int srcX0, int srcY0, int srcX1, int srcY1,
	int dstX0, int dstY0, int dstX1, int dstY1,
	CopyAttachmentMask mask,
	CopyAttachmentFilter filter
) {
	// Preserve current framebuffer binding
	GLint drawFramebufferId = 0, readframebufferId = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFramebufferId);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readframebufferId);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebufferPointer);
	glReadBuffer(m_attachmentsTypesMap.at(attachment));

	glBindFramebuffer(
		GL_DRAW_FRAMEBUFFER, 
		static_cast<OpenGL3Framebuffer*>(destination)->getFramebufferPointer()
	);
	glDrawBuffer(m_attachmentsTypesMap.at(attachment));

	glBlitFramebuffer(
		srcX0, srcY0, srcX1, srcY1, 
		dstX0, dstY0, dstX1, dstY1, 
		m_copyAttachmentMasksMap.at(mask), 
		((filter == CopyAttachmentFilter::Linear) ? GL_LINEAR : GL_NEAREST)
	);

	// Restore framebuffer binding
	glBindFramebuffer(GL_READ_FRAMEBUFFER, readframebufferId);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFramebufferId);
}

void OpenGL3Framebuffer::disableColorComponent() {
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

bool OpenGL3Framebuffer::hasTextureAttachment(Attachment attachment) const {
	return m_attachedTextures.find(attachment) != m_attachedTextures.end();
}

Texture* OpenGL3Framebuffer::getTextureAttachment(Attachment attachment) const {
	return m_attachedTextures.at(attachment);
}