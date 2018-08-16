#include "OpenGL3WindowRenderTarget.h"

#include <Engine\Components\Graphics\RenderSystem\RenderSystemException.h>

OpenGL3WindowRenderTarget::OpenGL3WindowRenderTarget()
	: OpenGL3RenderTarget()
{
}

OpenGL3WindowRenderTarget::~OpenGL3WindowRenderTarget()
{
}

void OpenGL3WindowRenderTarget::create()
{
	m_frameBuffer = 0;
}

void OpenGL3WindowRenderTarget::destroy()
{
}

void OpenGL3WindowRenderTarget::attachColorComponent(size_t index, Texture * texture)
{
	throw RenderSystemException("Attaching component to window render target is forbidden", __FILE__, __LINE__, __FUNCTION__);
}

void OpenGL3WindowRenderTarget::attachDepthStencilComponent(Texture * texture)
{
	throw RenderSystemException("Attaching component to window render target is forbidden", __FILE__, __LINE__, __FUNCTION__);
}