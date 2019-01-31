#include "GraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(GraphicsContext* graphicsContext)
	: m_graphicsContext(graphicsContext)
{

	m_gBufferP0 = createGBufferColorTexture();
	m_gBufferP1 = createGBufferColorTexture();
	m_gBufferP2 = createGBufferColorTexture();

	m_gBufferDepthStencil = createGBufferDepthStencilTexture();

	m_gBufferTarget = m_graphicsContext->createRenderTarget();
	m_gBufferTarget->create();

	m_gBufferTarget->bind();
	m_gBufferTarget->attachColorComponent(0, m_gBufferP0);
	m_gBufferTarget->attachColorComponent(1, m_gBufferP1);
	m_gBufferTarget->attachColorComponent(2, m_gBufferP2);
	m_gBufferTarget->attachDepthStencilComponent(m_gBufferDepthStencil);
	m_gBufferTarget->unbind();

}

GraphicsPipeline::~GraphicsPipeline()
{
}

RenderTarget * GraphicsPipeline::getGBuffer() const
{
	return m_gBufferTarget;
}

Texture * GraphicsPipeline::getGBufferAttachment0() const
{
	return m_gBufferP0;
}

Texture * GraphicsPipeline::getGBufferAttachment1() const
{
	return m_gBufferP1;
}

Texture * GraphicsPipeline::getGBufferAttachment2() const
{
	return m_gBufferP2;
}

Texture * GraphicsPipeline::getGBufferAttachmentDT() const
{
	return m_gBufferDepthStencil;
}

GraphicsContext * GraphicsPipeline::getGraphicsContext() const
{
	return m_graphicsContext;
}

Texture* GraphicsPipeline::createGBufferColorTexture()
{
	Texture* texture = m_graphicsContext->createTexture();

	texture->setTarget(Texture::Target::_2D);
	texture->setInternalFormat(Texture::InternalFormat::RGBA32F);
	texture->setSize(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight());
	texture->create();

	texture->bind();
	texture->setData(Texture::PixelFormat::RGBA, Texture::PixelDataType::Float, nullptr);
	texture->setMinificationFilter(Texture::Filter::Linear);
	texture->setMagnificationFilter(Texture::Filter::Linear);

	return texture;
}

Texture* GraphicsPipeline::createGBufferDepthStencilTexture()
{
	Texture* texture = m_graphicsContext->createTexture();

	texture->setTarget(Texture::Target::_2D);
	texture->setInternalFormat(Texture::InternalFormat::Depth24Stencil8);
	texture->setSize(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight());
	texture->create();

	texture->bind();
	texture->setData(Texture::PixelFormat::DepthStencil, Texture::PixelDataType::UnsignedInt24_8, nullptr);

	return texture;
}