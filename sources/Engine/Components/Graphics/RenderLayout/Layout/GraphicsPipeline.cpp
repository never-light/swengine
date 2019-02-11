#include "GraphicsPipeline.h"

GraphicsPipeline::GraphicsPipeline(GraphicsContext* graphicsContext)
	: m_graphicsContext(graphicsContext),
	m_isNormalMappingEnabled(true)
{
	unsigned int viewportWidth = graphicsContext->getViewportWidth();
	unsigned int viewportHeight = graphicsContext->getViewportHeight();

	m_gBufferP0 = createColorTexture(Texture::InternalFormat::R11FG11FB10F);
	m_gBufferP1 = createColorTexture();
	m_gBufferP2 = createColorTexture();

	m_gBufferDepthStencil = createDepthStencilTexture();

	m_gBufferTarget = m_graphicsContext->createRenderTarget();
	m_gBufferTarget->create();

	m_gBufferTarget->setSize(viewportWidth, viewportHeight);

	m_gBufferTarget->bind();
	m_gBufferTarget->attachColorComponent(0, m_gBufferP0);
	m_gBufferTarget->attachColorComponent(1, m_gBufferP1);
	m_gBufferTarget->attachColorComponent(2, m_gBufferP2);
	m_gBufferTarget->attachDepthStencilComponent(m_gBufferDepthStencil);
	m_gBufferTarget->unbind();

	m_hdrTexture = createColorTexture(Texture::InternalFormat::RGBA16F);
	m_hdrDepthStencil = createDepthStencilTexture();

	m_hrdTarget = m_graphicsContext->createRenderTarget();
	m_hrdTarget->create();

	m_hrdTarget->setSize(viewportWidth, viewportHeight);

	m_hrdTarget->bind();
	m_hrdTarget->attachColorComponent(0, m_hdrTexture);
	m_hrdTarget->attachDepthStencilComponent(m_hdrDepthStencil);
	m_hrdTarget->unbind();

	m_brightFilterTexture = createColorTexture(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight(),
		Texture::InternalFormat::RGBA16F);
	m_brightFilterTexture->setMinificationFilter(Texture::Filter::Linear);
	m_brightFilterTexture->setMagnificationFilter(Texture::Filter::Linear);

	m_blurredBrightTexture1 = createColorTexture(m_graphicsContext->getViewportWidth() / 4, m_graphicsContext->getViewportHeight() / 4,
		Texture::InternalFormat::RGBA16F);
	m_blurredBrightTexture1->setMinificationFilter(Texture::Filter::Linear);
	m_blurredBrightTexture1->setMagnificationFilter(Texture::Filter::Linear);


	m_blurredBrightTexture2 = createColorTexture(m_graphicsContext->getViewportWidth() / 4, m_graphicsContext->getViewportHeight() / 4,
		Texture::InternalFormat::RGBA16F);
	m_blurredBrightTexture2->generateMipMaps();

	m_blurredBrightTexture2->setMinificationFilter(Texture::Filter::LinearMipmapLinear);
	m_blurredBrightTexture2->setMagnificationFilter(Texture::Filter::Linear);


	m_brightFilterTarget = m_graphicsContext->createRenderTarget();
	m_brightFilterTarget->create();

	m_brightFilterTarget->setSize(viewportWidth, viewportHeight);

	m_brightFilterTarget->bind();
	m_brightFilterTarget->attachColorComponent(0, m_brightFilterTexture);
	m_brightFilterTarget->createDefaultDepthComponent();
	m_brightFilterTarget->unbind();

	m_blurTarget1 = m_graphicsContext->createRenderTarget();
	m_blurTarget1->create();

	m_blurTarget1->setSize(viewportWidth / 4, viewportHeight / 4);

	m_blurTarget1->bind();
	m_blurTarget1->attachColorComponent(0, m_blurredBrightTexture1);
	m_blurTarget1->createDefaultDepthComponent();
	m_blurTarget1->unbind();

	m_blurTarget2 = m_graphicsContext->createRenderTarget();
	m_blurTarget2->create();

	m_blurTarget2->setSize(viewportWidth / 4, viewportHeight / 4);

	m_blurTarget2->bind();
	m_blurTarget2->attachColorComponent(0, m_blurredBrightTexture2);
	m_blurTarget2->createDefaultDepthComponent();
	m_blurTarget2->unbind();
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
	delete m_hdrDepthStencil;

	delete m_brightFilterTexture;
	delete m_blurredBrightTexture1;
	delete m_blurredBrightTexture2;

	delete m_brightFilterTarget;
	delete m_blurTarget1;
	delete m_blurTarget2;
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

bool GraphicsPipeline::isNormalMappingEnabled() const
{
	return m_isNormalMappingEnabled;
}

void GraphicsPipeline::enableNormalMapping(bool enabled)
{
	m_isNormalMappingEnabled = enabled;
}

RenderTarget * GraphicsPipeline::getBlurRenderTarget1() const
{
	return m_blurTarget1;
}

RenderTarget * GraphicsPipeline::getBlurRenderTarget2() const
{
	return m_blurTarget2;
}

Texture * GraphicsPipeline::getBlurredBrightTexture1() const
{
	return m_blurredBrightTexture1;
}

Texture * GraphicsPipeline::getBlurredBrightTexture2() const
{
	return m_blurredBrightTexture2;
}

RenderTarget * GraphicsPipeline::getBrightFilterRenderTarget() const
{
	return m_brightFilterTarget;
}

Texture * GraphicsPipeline::getBrightFilterTexture() const
{
	return m_brightFilterTexture;
}

Texture* GraphicsPipeline::createColorTexture(Texture::InternalFormat internalFormat)
{
	return createColorTexture(m_graphicsContext->getViewportWidth(), m_graphicsContext->getViewportHeight(), internalFormat);
}

Texture * GraphicsPipeline::createColorTexture(unsigned int width, unsigned int height, Texture::InternalFormat internalFormat)
{
	Texture* texture = m_graphicsContext->createTexture();

	texture->setTarget(Texture::Target::_2D);
	texture->setInternalFormat(internalFormat);
	texture->setSize(width, height);
	texture->create();

	texture->bind();
	texture->setData(Texture::PixelFormat::RGBA, Texture::PixelDataType::Float, nullptr);
	texture->setMinificationFilter(Texture::Filter::Nearest);
	texture->setMagnificationFilter(Texture::Filter::Nearest);

	texture->setWrapMode(Texture::WrapMode::ClampToEdge);

	return texture;
}

Texture* GraphicsPipeline::createDepthStencilTexture()
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
	
	texture->setWrapMode(Texture::WrapMode::ClampToEdge);


	return texture;
}