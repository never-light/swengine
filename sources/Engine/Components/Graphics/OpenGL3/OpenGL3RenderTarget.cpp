#include "OpenGL3RenderTarget.h"

#include "OpenGL3Errors.h"
#include <Engine\assertions.h>

#include "OpenGL3Texture.h"

OpenGL3RenderTarget::OpenGL3RenderTarget()
	: RenderTarget(),
	m_frameBuffer(0),
	m_freeColorAttachment(0)
{
}

OpenGL3RenderTarget::~OpenGL3RenderTarget()
{
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

void OpenGL3RenderTarget::attachComponent(Component type, Texture * texture)
{
	OpenGL3Texture* glTexture = static_cast<OpenGL3Texture*>(texture);

	_assert(glTexture->getBindingTarget());

	GLenum attachmentType;

	if (type == Component::Color) {
		attachmentType = GL_COLOR_ATTACHMENT0 + m_freeColorAttachment;
		m_freeColorAttachment++;
	}
	else if (type == Component::Depth) {
		attachmentType = GL_DEPTH_ATTACHMENT;
	}
	else if (type == Component::DepthStencil) {
		attachmentType = GL_DEPTH_STENCIL_ATTACHMENT;
	}

	texture->bind();

	OPENGL3_CALL(glFramebufferTexture2D(
		GL_FRAMEBUFFER, 
		attachmentType, GL_TEXTURE_2D, 
		glTexture->getTexturePointer(),
		0));
}

void OpenGL3RenderTarget::copyComponentRawData(RenderTarget * destination, const Rect& sourceArea, const Rect& destinationArea, unsigned int copyMode, CopyFilter copyFilter) {
	GLuint destinationTargetPointer = 0;
	
	if (destination != nullptr) {
		OpenGL3RenderTarget* glDestinationTarget = static_cast<OpenGL3RenderTarget*>(destination);
		destinationTargetPointer = glDestinationTarget->getFrameBufferPointer();
	}
	
	GLbitfield glCopyMask = 0;

	if (copyMode & RenderTarget::COPY_COLOR)
		glCopyMask |= GL_COLOR_BUFFER_BIT;
	
	if (copyMode & RenderTarget::COPY_DEPTH)
		glCopyMask |= GL_DEPTH_BUFFER_BIT;

	if (copyMode & RenderTarget::COPY_STENCIL)
		glCopyMask |= GL_STENCIL_BUFFER_BIT;

	GLenum glCopyFilter;

	if (copyFilter == CopyFilter::Nearest)
		glCopyFilter = GL_NEAREST;
	else if (copyFilter == CopyFilter::Linear)
		glCopyFilter = GL_LINEAR;

	OPENGL3_CALL_BLOCK_BEGIN();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destinationTargetPointer);
		glBlitFramebuffer(
			sourceArea.getX(), sourceArea.getY(), sourceArea.getWidth(), sourceArea.getHeight(), 
			destinationArea.getX(), destinationArea.getY(), destinationArea.getWidth(), destinationArea.getHeight(),
			glCopyMask, glCopyFilter
		);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer);
	OPENGL3_CALL_BLOCK_END();
}

GLuint OpenGL3RenderTarget::getFrameBufferPointer() const
{
	return m_frameBuffer;
}
