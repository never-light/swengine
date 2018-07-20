#include "TextureLoader.h"

#include "HoldingResource.h"
#include <Engine\Components\Graphics\RenderSystem\Texture.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


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
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);

	if (!data) {
		throw std::exception(("Texture loading error, file is not available [" + filename + "]").c_str());
	}

	Texture::PixelFormat pixelFormat = Texture::PixelFormat::RGBA;

	Texture* texture = m_graphicsResourceFactory->createTexture();
	texture->setTarget(Texture::Target::_2D);
	texture->setInternalFormat(Texture::InternalFormat::RGBA);
	texture->setSize(width, height);

	texture->create();
	texture->bind();

	texture->setData(pixelFormat, Texture::PixelDataType::UnsignedByte, (const std::byte*)data);

	texture->generateMipMaps();
	texture->setMinificationFilter(Texture::Filter::LinearMipmapLinear);
	texture->setMagnificationFilter(Texture::Filter::Linear);

	texture->setWrapMode(Texture::WrapMode::Repeat);

	texture->unbind();

	stbi_image_free(data);

	return new HoldingResource<Texture>(texture);
}
