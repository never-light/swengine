#include "TextureLoader.h"

#include <Engine/Components/Graphics/RenderSystem/Texture.h>
#include <Engine/Components/Graphics/RenderSystem/RenderSystemException.h>

#include <stb_image.h>
#include "ResourceLoadingException.h"

TextureLoader::TextureLoader(GraphicsContext* graphicsContext)
	: ResourceLoader(), m_graphicsContext(graphicsContext)
{
}

TextureLoader::~TextureLoader()
{
}

BaseResourceInstance * TextureLoader::load(const std::string& path, std::optional<std::any> options)
{
	int width, height;
	int nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data == 0)
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

	Texture::PixelFormat pixelFormat;
	Texture::InternalFormat internalFormat;

	TextureLoadingOptions loadingOptions = 
		(options.has_value()) ? std::any_cast<TextureLoadingOptions>(options.value()) : TextureLoadingOptions();

	switch (nrChannels) {
	case 1:
		pixelFormat = Texture::PixelFormat::R;
		internalFormat = Texture::InternalFormat::R8;

		break;

	case 2:
		pixelFormat = Texture::PixelFormat::RG;
		internalFormat = Texture::InternalFormat::RG8;

		break;

	case 3:
		pixelFormat = Texture::PixelFormat::RGB;
		internalFormat = Texture::InternalFormat::RGB8;

		if (loadingOptions.format == TextureLoadingOptions::Format::sRGB)
			internalFormat = Texture::InternalFormat::SRGB8;

		break;

	case 4:
		pixelFormat = Texture::PixelFormat::RGBA;
		internalFormat = Texture::InternalFormat::RGBA8;

		if (loadingOptions.format == TextureLoadingOptions::Format::sRGB)
			internalFormat = Texture::InternalFormat::SRGBA8;

		break;
	}

	Texture* texture = nullptr;

	try {
		texture = m_graphicsContext->createTexture();
		texture->setTarget(Texture::Target::_2D);
		texture->setInternalFormat(internalFormat);
		texture->setSize(width, height);

		texture->create();
		texture->bind();

		texture->setData(pixelFormat, Texture::PixelDataType::UnsignedByte, (const std::byte*)data);

		texture->generateMipMaps();
		texture->setMinificationFilter(Texture::Filter::LinearMipmapLinear);
		texture->setMagnificationFilter(Texture::Filter::Linear);

		texture->setWrapMode(Texture::WrapMode::Repeat);

		texture->enableAnisotropicFiltering(16.0f);
	}
	catch (const RenderSystemException& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), exception.what(), exception.getFile(), exception.getLine(), exception.getFunction());
	}

	stbi_image_free(data);

	return new ResourceInstance<Texture>(texture);
}