#include "RawImageLoader.h"

#include "HoldingResource.h"
#include <Engine\Components\GUI\RawImage.h>

#include <stb_image.h>

RawImageLoader::RawImageLoader()
{
}

RawImageLoader::~RawImageLoader()
{
}

Resource * RawImageLoader::load(const std::string & filename)
{
	int width, height;
	int nrChannels;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (!data) {
		throw std::exception(("Image loading error, file is not available [" + filename + "]").c_str());
	}

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

	return new HoldingResource<RawImage>(image);
}