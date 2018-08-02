#include "TextureLoader.h"

#include "HoldingResource.h"
#include <Engine\Components\Graphics\RenderSystem\Texture.h>

#include <stb_image.h>
#include "ResourceLoadingException.h"

TextureLoader::TextureLoader(GraphicsResourceFactory* graphicsResourceFactory)
	: ResourceLoader(), m_graphicsResourceFactory(graphicsResourceFactory)
{
}

TextureLoader::~TextureLoader()
{
}

Resource* TextureLoader::load(const std::string & filename)
{
	int width, height;
	int nrChannels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (data == 0)
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

	Texture::PixelFormat pixelFormat;
	Texture::InternalFormat internalFormat;

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

		break;

	case 4:
		pixelFormat = Texture::PixelFormat::RGBA;
		internalFormat = Texture::InternalFormat::RGBA8;

		break;
	}

	Texture* texture = nullptr;

	try {
		texture = m_graphicsResourceFactory->createTexture();
		texture->setTarget(Texture::Target::_2D);
		texture->setInternalFormat(internalFormat);
		texture->setSize(width, height);

		texture->create();
		texture->bind();

		texture->setData(pixelFormat, Texture::PixelDataType::UnsignedByte, (const std::byte*)data);
	}
	catch (const RenderSystemException& exception) {
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, filename.c_str(), exception.what(), exception.getFile(), exception.getLine(), exception.getFunction());
	}

	stbi_image_free(data);

	return new HoldingResource<Texture>(texture);
}
