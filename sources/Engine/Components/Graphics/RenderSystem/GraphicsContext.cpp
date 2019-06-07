#include "GraphicsContext.h"

GraphicsContext::GraphicsContext(std::shared_ptr<sw::platform::base::Window> window, RenderTarget* windowRenderTarget,  Logger* logger)
	: m_window(window), 
	m_windowRenderTarget(windowRenderTarget),
	m_logger(logger)
{

}

GraphicsContext::~GraphicsContext() {
	if (m_windowRenderTarget != nullptr) {
		delete m_windowRenderTarget;
	}
}

unsigned int GraphicsContext::getViewportWidth() const
{
	return m_window->getWidth();
}

unsigned int GraphicsContext::getViewportHeight() const
{
	return m_window->getHeight();
}

RenderTarget * GraphicsContext::getWindowRenderTarget() const
{
	return m_windowRenderTarget;
}

void GraphicsContext::setClearColor(float r, float g, float b, float a)
{
	m_windowRenderTarget->setClearColor(r, g, b, a);
}

void GraphicsContext::setClearColor(const vector3 & color)
{
	m_windowRenderTarget->setClearColor(color);
}

void GraphicsContext::setClearColor(const vector4 & color)
{
	m_windowRenderTarget->setClearColor(color);
}

void GraphicsContext::setDepthClearValue(float depthValue)
{
	m_windowRenderTarget->setDepthClearValue(depthValue);
}

void GraphicsContext::setStencilClearValue(int stencilValue)
{
	m_windowRenderTarget->setStencilClearValue(stencilValue);
}

void GraphicsContext::clear(RTClearMode mode)
{
	m_windowRenderTarget->clear(mode);
}
