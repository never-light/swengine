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

private:
	Texture* createGBufferColorTexture(Texture::InternalFormat internalFormat = Texture::InternalFormat::RGBA8);
	Texture* createGBufferDepthStencilTexture();

private:
	GraphicsContext* m_graphicsContext;

	RenderTarget * m_gBufferTarget;

	Texture* m_gBufferP0;
	Texture* m_gBufferP1;
	Texture* m_gBufferP2;
	Texture* m_gBufferDepthStencil;

	RenderTarget* m_hrdTarget;

	Texture* m_hdrTexture;
	Texture* m_hdrDepthStencil;
};