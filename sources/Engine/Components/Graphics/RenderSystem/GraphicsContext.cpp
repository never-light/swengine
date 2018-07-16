#include "GraphicsContext.h"

GraphicsContext::GraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight)
	: m_window(window), m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight)
{

}

GraphicsContext::~GraphicsContext() {

}

unsigned int GraphicsContext::getViewportWidth() const
{
	return m_viewportWidth;
}

unsigned int GraphicsContext::getViewportHeight() const
{
	return m_viewportHeight;
}
