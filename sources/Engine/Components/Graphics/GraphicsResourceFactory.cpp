#include "GraphicsResourceFactory.h"

GraphicsResourceFactory* GraphicsResourceFactory::m_instance = nullptr;

GraphicsContext * GraphicsResourceFactory::createGraphicsContext(Window* window, unsigned int viewportWidth, unsigned int viewportHeight)
{
	return new OpenGL3GraphicsContext(window, viewportWidth, viewportHeight);
}

Material * GraphicsResourceFactory::createMaterial()
{
	return new OpenGL3Material();
}

Mesh * GraphicsResourceFactory::createMesh()
{
	return new OpenGL3Mesh();
}

Texture * GraphicsResourceFactory::createTexture()
{
	return new OpenGL3Texture();
}

GpuProgram * GraphicsResourceFactory::createGpuProgram()
{
	return new OpenGL3GpuProgram();
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
