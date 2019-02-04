#include "GraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(GraphicsContext* graphicsContext)
	: m_graphicsContext(graphicsContext)
{
	m_gBufferP0 = createGBufferColorTexture(Texture::InternalFormat::RG16F);
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

	m_hdrTexture = createGBufferColorTexture(Texture::InternalFormat::RGBA16F);
	m_hdrDepthStencil = createGBufferDepthStencilTexture();

	m_hrdTarget = m_graphicsContext->createRenderTarget();
	m_hrdTarget->create();

	m_hrdTarget->bind();
	m_hrdTarget->attachColorComponent(0, m_hdrTexture);
	m_hrdTarget->attachDepthStencilComponent(m_hdrDepthStencil);
	m_hrdTarget->unbind();
}

GraphicsPipeline::~GraphicsPipeline()
{
	delete m_gBufferP0;
	delete m_gBufferP1;
	delete m_gBufferP2;
	delete m_gBufferDepthStencil;

	delete m_gBufferTarget;

	delete m_hdrTexture;
	delete m_hrdTarget;
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

Texture * GraphicsPipeline::getGBufferAttachmentDS() const
{
	return m_gBufferDepthStencil;
}

GraphicsContext * GraphicsPipeline::getGraphicsContext() const
{
	return m_graphicsContext;
}

RenderTarget* GraphicsPipeline::getHDRBuffer() const
{
	return m_hrdTarget;
}

Texture * GraphicsPipeline::getHDRTexture() const
{
	return m_hdrTexture;
}

Texture * GraphicsPipeline::getHDRDS() const
{
	return m_hdrDepthStencil;
}

Texture* GraphicsPipeline::createGBufferColorTexture(Texture::InternalFormat internalFormat)
{
	Texture* texture = m_graphicsContext->createTexture();

	texture->setTarget(Texture::Target::_2D);
	texture->setInternalFormat(internalFormat);
	texture->setSize(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight());
	texture->create();

	texture->bind();
	texture->setData(Texture::PixelFormat::RGBA, Texture::PixelDataType::Float, nullptr);
	texture->setMinificationFilter(Texture::Filter::Nearest);
	texture->setMagnificationFilter(Texture::Filter::Nearest);

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

	texture->setMinificationFilter(Texture::Filter::Nearest);
	texture->setMagnificationFilter(Texture::Filter::Nearest);


	return texture;
}