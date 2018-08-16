#include "OpenGL3RenderTarget.h"

#include "OpenGL3Errors.h"
#include <Engine\assertions.h>

#include "OpenGL3Texture.h"

#include <Engine\Components\Graphics\RenderSystem\RenderSystemException.h>

OpenGL3RenderTarget::OpenGL3RenderTarget()
	: RenderTarget(),
	m_frameBuffer(0),
	m_freeColorAttachment(0),
	m_colorAttachments(new GLenum[MAX_COLOR_ATTACHMENTS]),
	m_isComplete(false)
{
}

OpenGL3RenderTarget::~OpenGL3RenderTarget()
{
	delete m_colorAttachments;

	if (m_frameBuffer != 0)
		destroy();
}

void OpenGL3RenderTarget::create()
{
	glGenFramebuffers(1, &m_frameBuffer);
}

void OpenGL3RenderTarget::destroy()
{
	if (m_frameBuffer != 0) {
		glDeleteFramebuffers(1, &m_frameBuffer);
		m_frameBuffer = 0;
	}
}

void OpenGL3RenderTarget::bind()
{
	OPENGL3_CALL(glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer));
}

void OpenGL3RenderTarget::unbind()
{
	if (!m_isComplete && m_freeColorAttachment != 0) {
		OPENGL3_CALL(glDrawBuffers(m_freeColorAttachment, m_colorAttachments));

		GLenum glFrameBufferState = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (glFrameBufferState != GL_FRAMEBUFFER_COMPLETE) {
			throw RenderSystemException("Framebuffer is not complete", __FILE__, __LINE__, __FUNCTION__);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGL3RenderTarget::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void OpenGL3RenderTarget::setClearColor(const vector3 & color)
{
	glClearColor(color.r, color.g, color.b, 1.0f);
}

void OpenGL3RenderTarget::setClearColor(const vector4 & color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGL3RenderTarget::setDepthClearValue(float depthValue)
{
	glClearDepth(depthValue);
}

void OpenGL3RenderTarget::setStencilClearValue(int stencilValue)
{
	glClearStencil(stencilValue);
}

void OpenGL3RenderTarget::clear(unsigned int mode)
{
	GLbitfield mask = 0;

	if (mode & RenderTarget::CLEAR_COLOR)
		mask |= GL_COLOR_BUFFER_BIT;

	if (mode & RenderTarget::CLEAR_DEPTH)
		mask |= GL_DEPTH_BUFFER_BIT;

	if (mode & RenderTarget::CLEAR_STENCIL)
		mask |= GL_STENCIL_BUFFER_BIT;

	glClear(mask);
}

void OpenGL3RenderTarget::attachColorComponent(size_t index, Texture * texture)
{
	OpenGL3Texture* glTexture = static_cast<OpenGL3Texture*>(texture);
	_assert(glTexture->getBindingTarget() == GL_TEXTURE_2D);

	GLenum attachmentType = GL_COLOR_ATTACHMENT0 + m_freeColorAttachment;
	m_colorAttachments[m_freeColorAttachment] = attachmentType;
	m_freeColorAttachment++;

	texture->bind();

	OPENGL3_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, glTexture->getTexturePointer(), 0));
}

void OpenGL3RenderTarget::attachDepthStencilComponent(Texture * texture)
{
	OpenGL3Texture* glTexture = static_cast<OpenGL3Texture*>(texture);
	_assert(glTexture->getBindingTarget() == GL_TEXTURE_2D);

	texture->bind();

	OPENGL3_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, glTexture->getTexturePointer(), 0));
}

void OpenGL3RenderTarget::copyColorComponentData(size_t sourceComponentIndex, 
	RenderTarget * destination, 
	size_t destinationComponentIndex,
	const Rect & sourceArea, 
	const Rect & destinationArea, 
	CopyFilter copyFilter)
{
	_assert(destination != nullptr || (destination == nullptr && destinationComponentIndex == 0));

	OpenGL3RenderTarget* glDestinationTarget = nullptr;
	GLuint destinationTargetPointer = 0;

	if (destination != nullptr) {
		glDestinationTarget = static_cast<OpenGL3RenderTarget*>(destination);
		destinationTargetPointer = glDestinationTarget->getFrameBufferPointer();
	}

	GLenum glCopyFilter;

	if (copyFilter == CopyFilter::Nearest)
		glCopyFilter = GL_NEAREST;
	else if (copyFilter == CopyFilter::Linear)
		glCopyFilter = GL_LINEAR;

	OPENGL3_CALL_BLOCK_BEGIN();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationTargetPointer);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + sourceComponentIndex);

	if (destinationTargetPointer != 0) {
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + destinationComponentIndex);
	}

	glBlitFramebuffer(
		sourceArea.getX(), sourceArea.getY(), sourceArea.getX() + sourceArea.getWidth(), sourceArea.getY() + sourceArea.getHeight(),
		destinationArea.getX(), destinationArea.getY(), destinationArea.getX() + destinationArea.getWidth(), destinationArea.getY() + destinationArea.getHeight(),
		GL_COLOR_BUFFER_BIT, glCopyFilter
	);
	
	if (destinationTargetPointer != 0) {
		glDrawBuffers(glDestinationTarget->m_freeColorAttachment, glDestinationTarget->m_colorAttachments);
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
	OPENGL3_CALL_BLOCK_END();
}

void OpenGL3RenderTarget::copyDepthStencilComponentData(RenderTarget * destination, const Rect & sourceArea, const Rect & destinationArea, CopyFilter copyFilter)
{
	OpenGL3RenderTarget* glDestinationTarget = nullptr;
	GLuint destinationTargetPointer = 0;

	if (destination != nullptr) {
		glDestinationTarget = static_cast<OpenGL3RenderTarget*>(destination);
		destinationTargetPointer = glDestinationTarget->getFrameBufferPointer();
	}

	GLenum glCopyFilter;

	if (copyFilter == CopyFilter::Nearest)
		glCopyFilter = GL_NEAREST;
	else if (copyFilter == CopyFilter::Linear)
		glCopyFilter = GL_LINEAR;

	OPENGL3_CALL_BLOCK_BEGIN();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationTargetPointer);
	glBlitFramebuffer(
		sourceArea.getX(), sourceArea.getY(), sourceArea.getX() + sourceArea.getWidth(), sourceArea.getY() + sourceArea.getHeight(),
		destinationArea.getX(), destinationArea.getY(), destinationArea.getX() + destinationArea.getWidth(), destinationArea.getY() + destinationArea.getHeight(),
		GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, glCopyFilter
	);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
	OPENGL3_CALL_BLOCK_END();
}

GLuint OpenGL3RenderTarget::getFrameBufferPointer() const
{
	return m_frameBuffer;
}
