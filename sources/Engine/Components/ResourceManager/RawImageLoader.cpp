#include "RawImageLoader.h"

#include "ResourceLoadingException.h"
#include <Engine/Components/ResourceManager/RawImage.h>

#include <stb_image.h>

RawImageLoader::RawImageLoader()
{
}

RawImageLoader::~RawImageLoader()
{
}

BaseResourceInstance* RawImageLoader::load(const std::string & path, std::optional<std::any> options)
{
	int width, height;
	int nrChannels;
	unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data == 0)
		throw ResourceLoadingException(ResourceLoadingError::InvalidData, path.c_str(), "", __FILE__, __LINE__, __FUNCTION__);

	std::byte* rawImageData = new std::byte[width * height * nrChannels];
	std::memcpy(rawImageData, data, width * height * nrChannels);

	stbi_image_free(data);

	RawImage::Format format = RawImage::Format::Invalid;

	if (nrChannels == 1)
		format = RawImage::Format::R8;
	else if (nrChannels == 2)
		format = RawImage::Format::RG8;
	else if (nrChannels == 3)
		format = RawImage::Format::RGB8;
	else if (nrChannels == 4)
		format = RawImage::Format::RGBA8;

	RawImage* image = new RawImage(format, width, height, rawImageData);

	return new ResourceInstance<RawImage>(image);
}