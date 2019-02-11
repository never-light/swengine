#include <Engine/assertions.h>

#include "GLRenderTarget.h"
#include "GLDebug.h"
#include "GLTexture.h"

#include <Engine/Components/Graphics/RenderSystem/RenderSystemException.h>

GLRenderTarget::GLRenderTarget()
	: RenderTarget(),
	m_frameBuffer(0),
	m_depthRenderBuffer(0),
	m_freeColorAttachment(0),
	m_width(0),
	m_height(0),
	m_colorAttachments(new GLenum[MAX_COLOR_ATTACHMENTS]),
	m_isComplete(false)
{
}

GLRenderTarget::~GLRenderTarget()
{
	delete m_colorAttachments;

	if (m_frameBuffer != 0)
		destroy();
}

void GLRenderTarget::setSize(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;
}

unsigned int GLRenderTarget::getWidth() const
{
	return m_width;
}

unsigned int GLRenderTarget::getHeight() const
{
	return m_height;
}

void GLRenderTarget::create()
{
	glGenFramebuffers(1, &m_frameBuffer);
}

void GLRenderTarget::destroy()
{
	if (m_frameBuffer != 0) {
		glDeleteFramebuffers(1, &m_frameBuffer);
		m_frameBuffer = 0;
	}

	if (m_depthRenderBuffer != 0) {
		glDeleteRenderbuffers(1, &m_depthRenderBuffer);
		m_depthRenderBuffer = 0;
	}
}

void GLRenderTarget::bind()
{
	GL_CALL_BLOCK_BEGIN();

	glViewport(0, 0, m_width, m_height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);

	GL_CALL_BLOCK_END();
}

void GLRenderTarget::unbind()
{
	if (!m_isComplete && m_freeColorAttachment != 0) {
		GL_CALL(glDrawBuffers(m_freeColorAttachment, m_colorAttachments));

		GLenum glFrameBufferState = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (glFrameBufferState != GL_FRAMEBUFFER_COMPLETE) {
			throw RenderSystemException("Framebuffer is not complete", __FILE__, __LINE__, __FUNCTION__);
		}

		m_isComplete = true;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLRenderTarget::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void GLRenderTarget::setClearColor(const vector3 & color)
{
	glClearColor(color.r, color.g, color.b, 1.0f);
}

void GLRenderTarget::setClearColor(const vector4 & color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void GLRenderTarget::setDepthClearValue(float depthValue)
{
	glClearDepthf(depthValue);
}

void GLRenderTarget::setStencilClearValue(int stencilValue)
{
	glClearStencil(stencilValue);
}

void GLRenderTarget::clear(RTClearMode mode)
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

void GLRenderTarget::attachColorComponent(size_t index, Texture * texture)
{
	GLTexture* glTexture = static_cast<GLTexture*>(texture);
	_assert(glTexture->getBindingTarget() == GL_TEXTURE_2D);

	GLenum attachmentType = GL_COLOR_ATTACHMENT0 + m_freeColorAttachment;
	m_colorAttachments[m_freeColorAttachment] = attachmentType;
	m_freeColorAttachment++;

	texture->bind();

	GL_CALL(glNamedFramebufferTexture(m_frameBuffer, attachmentType, glTexture->getTexturePointer(), 0));
}

void GLRenderTarget::attachDepthStencilComponent(Texture * texture)
{
	GLTexture* glTexture = static_cast<GLTexture*>(texture);
	_assert(glTexture->getBindingTarget() == GL_TEXTURE_2D);

	texture->bind();

	GL_CALL(glNamedFramebufferTexture(m_frameBuffer, GL_DEPTH_STENCIL_ATTACHMENT, glTexture->getTexturePointer(), 0));
}

void GLRenderTarget::attachDepthComponent(Texture * texture)
{
	GLTexture* glTexture = static_cast<GLTexture*>(texture);
	_assert(glTexture->getBindingTarget() == GL_TEXTURE_2D);

	texture->bind();

	GL_CALL(glNamedFramebufferTexture(m_frameBuffer, GL_DEPTH_ATTACHMENT, glTexture->getTexturePointer(), 0));
}

void GLRenderTarget::createDefaultDepthComponent()
{
	_assert(m_width != 0 && m_height != 0);

	unsigned int m_depthRenderBuffer;
	glGenRenderbuffers(1, &m_depthRenderBuffer);

	glNamedRenderbufferStorage(m_depthRenderBuffer, GL_DEPTH24_STENCIL8, m_width, m_height);
	glNamedFramebufferRenderbuffer(m_frameBuffer, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderBuffer);
}

void GLRenderTarget::copyColorComponentData(size_t sourceComponentIndex, 
	RenderTarget * destination, 
	size_t destinationComponentIndex,
	const Rect & sourceArea, 
	const Rect & destinationArea, 
	CopyFilter copyFilter)
{
	_assert(destination != nullptr || (destination == nullptr && destinationComponentIndex == 0));

	GLRenderTarget* glDestinationTarget = nullptr;
	GLuint destinationTargetPointer = 0;

	if (destination != nullptr) {
		glDestinationTarget = static_cast<GLRenderTarget*>(destination);
		destinationTargetPointer = glDestinationTarget->getFrameBufferPointer();
	}

	GLenum glCopyFilter;

	if (copyFilter == CopyFilter::Nearest)
		glCopyFilter = GL_NEAREST;
	else if (copyFilter == CopyFilter::Linear)
		glCopyFilter = GL_LINEAR;

	GL_CALL_BLOCK_BEGIN();

	glNamedFramebufferReadBuffer(m_frameBuffer, GL_COLOR_ATTACHMENT0 + sourceComponentIndex);

	if (destinationTargetPointer != 0) {
		glNamedFramebufferDrawBuffer(destinationTargetPointer, GL_COLOR_ATTACHMENT0 + destinationComponentIndex);
	}

	glBlitNamedFramebuffer(
		m_frameBuffer, destinationTargetPointer,
		sourceArea.getX(), sourceArea.getY(), sourceArea.getX() + sourceArea.getWidth(), sourceArea.getY() + sourceArea.getHeight(),
		destinationArea.getX(), destinationArea.getY(), destinationArea.getX() + destinationArea.getWidth(), destinationArea.getY() + destinationArea.getHeight(),
		GL_COLOR_BUFFER_BIT, glCopyFilter
	);

	GL_CALL_BLOCK_END();
}

void GLRenderTarget::copyDepthStencilComponentData(RenderTarget * destination, const Rect & sourceArea, const Rect & destinationArea, CopyFilter copyFilter)
{
	GLRenderTarget* glDestinationTarget = nullptr;
	GLuint destinationTargetPointer = 0;

	if (destination != nullptr) {
		glDestinationTarget = static_cast<GLRenderTarget*>(destination);
		destinationTargetPointer = glDestinationTarget->getFrameBufferPointer();
	}

	GLenum glCopyFilter;

	if (copyFilter == CopyFilter::Nearest)
		glCopyFilter = GL_NEAREST;
	else if (copyFilter == CopyFilter::Linear)
		glCopyFilter = GL_LINEAR;

	GL_CALL_BLOCK_BEGIN();
	glBlitNamedFramebuffer(
		m_frameBuffer, destinationTargetPointer,
		sourceArea.getX(), sourceArea.getY(), sourceArea.getX() + sourceArea.getWidth(), sourceArea.getY() + sourceArea.getHeight(),
		destinationArea.getX(), destinationArea.getY(), destinationArea.getX() + destinationArea.getWidth(), destinationArea.getY() + destinationArea.getHeight(),
		GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, glCopyFilter
	);
	GL_CALL_BLOCK_END();
}

GLuint GLRenderTarget::getFrameBufferPointer() const
{
	return m_frameBuffer;
}
