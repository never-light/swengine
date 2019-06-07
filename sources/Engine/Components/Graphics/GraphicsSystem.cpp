#include "GraphicsSystem.h"

#include <Engine/Components/Graphics/OpenGL4/GLGraphicsContext.h>
#include <Engine/Components/Graphics/OpenGL4/GLWindowRenderTarget.h>
#include <Engine/Components/Graphics/OpenGL4/GLGraphicsDevice.h>

GraphicsSystem::GraphicsSystem(std::shared_ptr<sw::platform::base::Window> window, sw::GraphicsAPI api, Logger* logger)
	: m_window(window)
{
	m_graphicsDevice = std::make_unique<GLGraphicsDevice>(window, logger);
}

GraphicsSystem::~GraphicsSystem() {
}

std::shared_ptr<GraphicsContext> GraphicsSystem::getImmediateGraphicsContext() const
{
	return m_graphicsDevice->getImmediateContext();
}

GraphicDevice* GraphicsSystem::getGraphicDevice() const
{
	return m_graphicsDevice.get();
}

void GraphicsSystem::update(float delta)
{
}
