#pragma once

#include <Engine/Components/Graphics/RenderSystem/Texture.h>
#include <Engine/Components/Graphics/RenderSystem/RenderTarget.h>
#include <Engine/Components/Graphics/RenderSystem/GraphicsContext.h>

class GraphicsPipeline {
public:
	GraphicsPipeline(GraphicsContext* graphicsContext);
	~GraphicsPipeline();

	RenderTarget* getGBuffer() const;

	Texture* getGBufferAttachment0() const;
	Texture* getGBufferAttachment1() const;
	Texture* getGBufferAttachment2() const;
	Texture* getGBufferAttachmentDS() const;
	 
	GraphicsContext* getGraphicsContext() const;

	RenderTarget* getHDRBuffer() const;

	Texture* getHDRTexture() const;
	Texture* getHDRDS() const;

	bool isNormalMappingEnabled() const;
	void enableNormalMapping(bool enabled = true);

	RenderTarget* getBlurRenderTarget1() const;
	RenderTarget* getBlurRenderTarget2() const;
	Texture* getBlurredBrightTexture1() const;
	Texture* getBlurredBrightTexture2() const;

	RenderTarget* getBrightFilterRenderTarget() const;
	Texture* getBrightFilterTexture() const;

private:
	Texture* createColorTexture(Texture::InternalFormat internalFormat = Texture::InternalFormat::RGBA8);
	Texture* createColorTexture(unsigned int width, unsigned int height, Texture::InternalFormat internalFormat);

	Texture* createDepthStencilTexture();

private:
	GraphicsContext* m_graphicsContext;

	// Geometry buffer
	RenderTarget * m_gBufferTarget;

	Texture* m_gBufferP0;
	Texture* m_gBufferP1;
	Texture* m_gBufferP2;
	Texture* m_gBufferDepthStencil;

	// HDR
	RenderTarget* m_hrdTarget;

	Texture* m_hdrTexture;
	Texture* m_hdrDepthStencil;

	// Bright filter
	RenderTarget* m_brightFilterTarget;

	Texture* m_brightFilterTexture;

	// Blur
	RenderTarget* m_blurTarget1;
	RenderTarget* m_blurTarget2;

	Texture* m_blurredBrightTexture1;
	Texture* m_blurredBrightTexture2;

private:
	bool m_isNormalMappingEnabled;
};