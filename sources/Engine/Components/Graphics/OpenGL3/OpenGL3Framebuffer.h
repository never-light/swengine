#pragma once

#include <unordered_map>
#include <Engine\Components\Graphics\RenderSystem\Framebuffer.h>
#include "OpenGL3.h"

class OpenGL3Framebuffer : public Framebuffer {
public:
	OpenGL3Framebuffer();
	~OpenGL3Framebuffer();

	GLuint getFramebufferPointer() const;

	void lock() override;
	void unlock() override;

	void attachTexture(Attachment attachment, Texture* texture) override;

	void createAndAttachRenderBuffer(
		Attachment attachment,
		int width,
		int height,
		RenderBufferInternalFormat internalFormat
	) override;

	void createAndAttachMultisampleRenderBuffer(
		Attachment attachment,
		int samples,
		int width,
		int height,
		RenderBufferInternalFormat internalFormat
	) override;

	void copyAttachmentFragmentTo(
		Attachment attachment,
		Framebuffer* destination,
		int srcX0, int srcY0, int srcX1, int srcY1,
		int dstX0, int dstY0, int dstX1, int dstY1,
		CopyAttachmentMask mask,
		CopyAttachmentFilter filter
	) override;

	bool hasTextureAttachment(Attachment attachment) const override;
	Texture* getTextureAttachment(Attachment attachment) const override;
private:
	GLuint createRenderBuffer(
		int width, int height,
		RenderBufferInternalFormat internalFormat,
		bool multisample = false, int samples = 0
	);

	void attachRenderBuffer(Attachment attachment, GLuint renderBufferPointer);

	void freeData();
private:
	std::unordered_map<Attachment, Texture*> m_attachedTextures;
	std::unordered_map<Attachment, GLuint> m_attachedRenderbuffers;

	GLuint m_framebufferPointer;

private:
	static const std::unordered_map<Framebuffer::Attachment, GLenum> m_attachmentsTypesMap;
	static const std::unordered_map<Framebuffer::RenderBufferInternalFormat, GLenum> m_renderBufferInternalFormatsMap;
	static const std::unordered_map<Framebuffer::CopyAttachmentMask, GLbitfield> m_copyAttachmentMasksMap;
};