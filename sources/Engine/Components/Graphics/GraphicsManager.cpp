#include "GraphicsManager.h"
#include <Engine\Components\Graphics\OpenGL3\OpenGL3Renderer.h>

GraphicsManager::GraphicsManager(Window* window, GraphicsDriver driver) 
	: m_window(window) 
{
	m_renderer = new OpenGL3Renderer(window);
}
GraphicsManager::~GraphicsManager() {
	if (m_renderer != nullptr)
		delete m_renderer;
}

Renderer* GraphicsManager::getRenderer() const {
	return m_renderer;
}

Material* GraphicsManager::createMaterial() {
	return new Material();
}

Texture* GraphicsManager::createTexture(Texture::Type type) {
	return new OpenGL3Texture(type);
}

GpuProgram* GraphicsManager::createGpuProgram(const std::unordered_map<std::string, std::string>& sources) {
	return new OpenGL3GpuProgram(sources);
}

Sprite* GraphicsManager::createSprite(Texture* texture, GpuProgram* gpuProgram) {
	return new OpenGL3Sprite(texture, gpuProgram);
}

Light* GraphicsManager::createLight(LightType type) {
	return new Light(type);
}

Framebuffer* GraphicsManager::createFramebuffer() {
	return new OpenGL3Framebuffer();
}

HardwareBuffer* GraphicsManager::createHardwareBuffer() {
	return new OpenGL3HardwareBuffer();
}