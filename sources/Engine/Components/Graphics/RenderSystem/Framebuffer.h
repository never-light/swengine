#pragma once

#include <unordered_map>
#include "Texture.h"

class Framebuffer {
public:
	enum class Attachment {
		Color0, Color1, Color2, Deptch, Stencil, DepthStencil
	};

	enum class RenderBufferInternalFormat {
		Depth, Depth24Stencil8
	};

	enum class CopyAttachmentFilter {
		Nearest, Linear
	};

	enum class CopyAttachmentMask {
		ColorBuffer, DepthBuffer, StencilBuffer, DepthStencilBuffer
	};
public:
	Framebuffer() {}
	virtual ~Framebuffer() {}

	virtual void lock() = 0;
	virtual void unlock() = 0;

	virtual void attachTexture(Attachment attachment, Texture* texture) = 0;
	virtual void createAndAttachRenderBuffer(
		Attachment attachment, 
		int width, 
		int height, 
		RenderBufferInternalFormat internalFormat
	) = 0;

	virtual void createAndAttachMultisampleRenderBuffer(
		Attachment attachment,
		int samples,
		int width,
		int height,
		RenderBufferInternalFormat internalFormat
	) = 0;

	virtual void copyAttachmentFragmentTo(
		Attachment attachment,
		Framebuffer* destination,
		int srcX0, int srcY0, int srcX1, int srcY1,
		int dstX0, int dstY0, int dstX1, int dstY1,
		CopyAttachmentMask mask,
		CopyAttachmentFilter filter
	) = 0;

	virtual void disableColorComponent() = 0;

	virtual bool hasTextureAttachment(Attachment attachment) const = 0;
	virtual Texture* getTextureAttachment(Attachment attachment) const = 0;
};