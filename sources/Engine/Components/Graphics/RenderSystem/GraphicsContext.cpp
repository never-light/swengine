#include "GraphicsContext.h"

GraphicsContext::GraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight, Logger* logger)
	: m_window(window), m_viewportWidth(viewportWidth), m_viewportHeight(viewportHeight), m_logger(logger)
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
