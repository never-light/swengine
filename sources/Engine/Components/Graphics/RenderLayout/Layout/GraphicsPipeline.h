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
	Texture* getGBufferAttachmentDT() const;

	GraphicsContext* getGraphicsContext() const;

private:
	Texture* createGBufferColorTexture();
	Texture* createGBufferDepthStencilTexture();

private:
	GraphicsContext* m_graphicsContext;

	RenderTarget * m_gBufferTarget;

	Texture* m_gBufferP0;
	Texture* m_gBufferP1;
	Texture* m_gBufferP2;
	Texture* m_gBufferDepthStencil;
};