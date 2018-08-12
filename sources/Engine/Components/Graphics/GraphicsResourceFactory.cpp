#include "GraphicsResourceFactory.h"

#include <Engine\Components\Graphics\OpenGL3\OpenGL3WindowRenderTarget.h>

GraphicsResourceFactory* GraphicsResourceFactory::m_instance = nullptr;

GraphicsContext * GraphicsResourceFactory::createGraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight, Logger* logger)
{
	RenderTarget* windowRenderTarget = new OpenGL3WindowRenderTarget();
	windowRenderTarget->create();

	return new OpenGL3GraphicsContext(window, viewportWidth, viewportHeight, windowRenderTarget, logger);
}

Buffer * GraphicsResourceFactory::createBuffer(Buffer::Type type, Buffer::Usage usage)
{
	return new OpenGL3Buffer(type, usage);
}

GeometryStore * GraphicsResourceFactory::createGeometryStore()
{
	return new OpenGL3GeometryStore();
}

Texture * GraphicsResourceFactory::createTexture()
{
	return new OpenGL3Texture();
}

GpuProgram * GraphicsResourceFactory::createGpuProgram()
{
	return new OpenGL3GpuProgram();
}

RenderTarget * GraphicsResourceFactory::createRenderTarget()
{
	return new OpenGL3RenderTarget();
}

void GraphicsResourceFactory::initialize(GraphicsAPI api)
{
	if (m_instance == nullptr)
		m_instance = new GraphicsResourceFactory(api);
}

void GraphicsResourceFactory::destroy()
{
	if (m_instance)
		delete m_instance;
}

GraphicsResourceFactory * GraphicsResourceFactory::getInstance()
{
	return m_instance;
}

GraphicsResourceFactory::GraphicsResourceFactory(GraphicsAPI api)
	: m_api(api)
{
}
