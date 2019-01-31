#include "GLWindowRenderTarget.h"

#include <Engine/Components/Graphics/RenderSystem/RenderSystemException.h>

GLWindowRenderTarget::GLWindowRenderTarget()
	: GLRenderTarget()
{
}

GLWindowRenderTarget::~GLWindowRenderTarget()
{
}

void GLWindowRenderTarget::create()
{
	m_frameBuffer = 0;
}

void GLWindowRenderTarget::destroy()
{
}

void GLWindowRenderTarget::attachColorComponent(size_t index, Texture * texture)
{
	throw RenderSystemException("Attaching component to window render target is forbidden", __FILE__, __LINE__, __FUNCTION__);
}

void GLWindowRenderTarget::attachDepthStencilComponent(Texture * texture)
{
	throw RenderSystemException("Attaching component to window render target is forbidden", __FILE__, __LINE__, __FUNCTION__);
}