#include "GraphicsSystem.h"

#include <Engine/Components/Graphics/OpenGL4/GLGraphicsContext.h>
#include <Engine/Components/Graphics/OpenGL4/GLWindowRenderTarget.h>

GraphicsSystem::GraphicsSystem(Window* window, GraphicsAPI api, Logger* logger)
	: m_window(window)
{
	RenderTarget* windowRenderTarget = new GLWindowRenderTarget();
	windowRenderTarget->create();

	m_graphicsContext = new GLGraphicsContext(window, window->getWidth(), window->getHeight(), windowRenderTarget, logger);
}

GraphicsSystem::~GraphicsSystem() {
	delete m_graphicsContext;
}

GraphicsContext * GraphicsSystem::getGraphicsContext() const
{
	return m_graphicsContext;
}

void GraphicsSystem::update(float delta)
{
}