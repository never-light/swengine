#include "GraphicsSystem.h"

GraphicsSystem::GraphicsSystem(Window* window, GraphicsAPI api, Logger* logger)
	: m_window(window), m_resourceFactory(nullptr)
{
	GraphicsResourceFactory::initialize(api);
	m_resourceFactory = GraphicsResourceFactory::getInstance();

	m_graphicsContext = m_resourceFactory->createGraphicsContext(window, window->getWidth(), window->getHeight(), logger);
}

GraphicsSystem::~GraphicsSystem() {
	delete m_graphicsContext;

	GraphicsResourceFactory::destroy();
}

GraphicsResourceFactory * GraphicsSystem::getResourceFactory() const
{
	return m_resourceFactory;
}

GraphicsContext * GraphicsSystem::getGraphicsContext() const
{
	return m_graphicsContext;
}
